#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>

#include <linux/netlink.h>
#include <linux/rtnetlink.h>
#include <linux/netdevice.h>

#include <net/sock.h>

#define DRV_NAME "nlmon"

static struct sock *nl_sk;

/*
 * Parse RTM_NEWLINK messages
 */
static void handle_link_event(struct nlmsghdr *nlh)
{
    struct ifinfomsg *ifi;
    struct rtattr *attr;
    int attrlen;

    char ifname[IFNAMSIZ] = {0};

    ifi = nlmsg_data(nlh);

    /*
     * Walk attributes
     */
    attr = IFLA_RTA(ifi);
    attrlen = IFLA_PAYLOAD(nlh);

    while (RTA_OK(attr, attrlen)) {

        switch (attr->rta_type) {

        case IFLA_IFNAME:
            strscpy(ifname,
                    RTA_DATA(attr),
                    IFNAMSIZ);
            break;
        }

        attr = RTA_NEXT(attr, attrlen);
    }

    pr_info("%s: index=%d flags=0x%x\n",
            ifname,
            ifi->ifi_index,
            ifi->ifi_flags);

    /*
     * Administrative state
     */
    if (ifi->ifi_flags & IFF_UP)
        pr_info("%s: ADMIN UP\n", ifname);
    else
        pr_info("%s: ADMIN DOWN\n", ifname);

    /*
     * Physical carrier state
     */
    if (ifi->ifi_flags & IFF_RUNNING)
        pr_info("%s: LINK UP\n", ifname);
    else
        pr_info("%s: LINK DOWN\n", ifname);
}

/*
 * Netlink receive callback
 */
static void nl_recv_msg(struct sk_buff *skb)
{
    struct nlmsghdr *nlh;

    /*
     * Iterate all netlink messages in skb
     */
    for (nlh = nlmsg_hdr(skb);
         nlmsg_ok(nlh, skb->len);
         nlh = nlmsg_next(nlh, &(int){ skb->len })) {

        switch (nlh->nlmsg_type) {

        case RTM_NEWLINK:
            handle_link_event(nlh);
            break;

        case RTM_DELLINK:
            pr_info("interface removed\n");
            break;

        default:
            break;
        }
    }
}

static int __init nlmon_init(void)
{
    struct netlink_kernel_cfg cfg = {
        .groups = RTNLGRP_LINK,
        .input  = nl_recv_msg,
    };

    pr_info(DRV_NAME ": init\n");

    nl_sk = netlink_kernel_create(&init_net,
                                  NETLINK_ROUTE,
                                  &cfg);

    if (!nl_sk) {
        pr_err(DRV_NAME ": failed to create netlink socket\n");
        return -ENOMEM;
    }

    pr_info(DRV_NAME ": listening for link events\n");

    return 0;
}

static void __exit nlmon_exit(void)
{
    pr_info(DRV_NAME ": exit\n");

    if (nl_sk)
        netlink_kernel_release(nl_sk);
}

module_init(nlmon_init);
module_exit(nlmon_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("example");
MODULE_DESCRIPTION("Netlink interface monitor");
