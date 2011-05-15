#include <linux/fs.h>
#include <linux/kernel.h>
#include <linux/mm.h>
#include <linux/module.h>
#include <linux/printk.h>
#include <linux/slab.h>

#define PHUANG_SUPER_MAGIC 0x9456

static int phuang_test_super(struct super_block *sb, void *data)
{
    return sb->s_fs_info == data;
}

static int phuang_set_super(struct super_block *sb, void *data)
{
    sb->s_fs_info = data;
    return set_anon_super(sb, NULL);
}

static struct kmem_cache *phuang_inode_cache;

static struct inode *phuang_alloc_inode(struct super_block *sb)
{
    struct inode *inode;

    inode = (struct inode *)kmem_cache_alloc(phuang_inode_cache, GFP_KERNEL);

    inode->i_mtime = inode->i_atime = inode->i_ctime = CURRENT_TIME;
    return inode;
}

static void phuang_destroy_inode(struct inode *inode)
{
    kmem_cache_free(phuang_inode_cache, inode);
}

static void phuang_evict_inode(struct inode *inode)
{
}

static const struct super_operations phuang_sops = {
    .alloc_inode    = phuang_alloc_inode,
    .destroy_inode  = phuang_destroy_inode,
    .drop_inode     = generic_delete_inode,
    .evict_inode    = phuang_evict_inode,
    .statfs         = simple_statfs,
};

static int phuang_fill_super(struct super_block *sb)
{
    struct inode *inode;

    sb->s_flags |= MS_NODIRATIME | MS_NOSUID | MS_NOEXEC;
    sb->s_blocksize = 1024;
    sb->s_blocksize_bits = 10;
    sb->s_magic = PHUANG_SUPER_MAGIC;
    sb->s_op = &phuang_sops;
    sb->s_time_gran = 1;

    inode = iget_locked(sb, 8989);
    if (inode == NULL)
        return -ENOMEM;

    if (inode->i_state & I_NEW) {
        inode->i_mtime = inode->i_atime = inode->i_ctime = CURRENT_TIME;
    }
    return 0;
}

static struct dentry *phuang_mount(struct file_system_type *fs_type,
        int flags, const char *dev_name, void *data)
{
    int err;
    struct super_block *sb;

    sb = sget(fs_type, phuang_test_super, phuang_set_super, data);
    if (IS_ERR(sb))
        return ERR_CAST(sb);

    if (sb->s_root == NULL) {
        sb->s_flags = flags;
        err = phuang_fill_super(sb);
        if (err) {
            deactivate_locked_super(sb);
            return ERR_PTR(err);
        }
        sb->s_flags |= MS_ACTIVE;
    }

    return dget(sb->s_root);
}

static void phuang_kill_sb(struct super_block *sb)
{
}

static struct file_system_type phuang_fs_type = {
    .owner      = THIS_MODULE,
    .name       = "phuang",
    .mount      = phuang_mount,
    .kill_sb    = phuang_kill_sb,
};

static int __init init_phuang_fs(void)
{ 
    int err;
    printk (KERN_DEBUG "Hello");
    
    phuang_inode_cache = kmem_cache_create("phuang_indoe_cache",
            sizeof (struct inode),
            0,
            (SLAB_RECLAIM_ACCOUNT | SLAB_MEM_SPREAD | SLAB_PANIC),
            (void(*)(void *))inode_init_once);

    err = register_filesystem(&phuang_fs_type);
    return err;
}

static void __exit exit_phuang_fs(void)
{
    unregister_filesystem(&phuang_fs_type);
    printk (KERN_DEBUG "Bye");
}

module_init(init_phuang_fs);
module_exit(exit_phuang_fs);

