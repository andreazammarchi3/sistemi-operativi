#include <linux/module.h>
#include <net/sock.h>
#include <linux/netlink.h>
#include <linux/skbuff.h>

#define NETLINK_USER 31

struct sock *nl_sk = NULL;

static void using_netlink_nl_recv_msg(struct sk_buff *skb)
{

    struct nlmsghdr *nlh;
    int pid;
    struct sk_buff *skb_out;
    int msg_size;
    char *msg = "Hello from kernel";
    int res;

    printk(KERN_INFO "Entering: %s\n", __FUNCTION__ );

    msg_size = strlen(msg);

    nlh = (struct nlmsghdr *)skb->data;
    printk(KERN_INFO "Netlink received msg payload: %s\n", (char *)nlmsg_data(nlh));
    pid = nlh->nlmsg_pid; /*pid of sending process */

    skb_out = nlmsg_new(msg_size, 0);
    if (!skb_out) {
        printk(KERN_ERR "Failed to allocate new skb\n");
        return;
    }

    nlh = nlmsg_put(skb_out, 0, 0, NLMSG_DONE, msg_size, 0);
    NETLINK_CB(skb_out).dst_group = 0; /* not in mcast group */
    strncpy(nlmsg_data(nlh), msg, msg_size);

    res = nlmsg_unicast(nl_sk, skb_out, pid);
    if (res < 0)
        printk(KERN_INFO "Error while sending back to user\n");
    else
        printk(KERN_INFO "kernel module Sent back to user\n");
}

static int __init using_netlink_init(void)
{
    /* for kernel 3.6 and above */
    struct netlink_kernel_cfg cfg = {
         .groups = 1,
         .input = using_netlink_nl_recv_msg,
    };

    printk("Entering: %s\n", __FUNCTION__);

    /* for kernel below 3.6
    nl_sk = netlink_kernel_create(&init_net, NETLINK_USER, 0, using_netlink_nl_recv_msg,
                                  NULL, THIS_MODULE);
    */
    /* for kernel 3.6 and above */
    nl_sk = netlink_kernel_create(&init_net, NETLINK_USER, &cfg);
    /* */
    if (!nl_sk) {
        printk(KERN_ALERT "Error creating socket.\n");
        return -10;
    }

    return 0;
}

static void __exit using_netlink_exit(void)
{

    printk(KERN_INFO "exiting using_netlink module\n");
    netlink_kernel_release(nl_sk);
}

module_init(using_netlink_init);
module_exit(using_netlink_exit);

MODULE_LICENSE("GPL");
