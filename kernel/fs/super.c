#include <linux/fs.h>
#include <linux/kernel.h>
#include <linux/mm.h>
#include <linux/module.h>
#include <linux/printk.h>
#include <linux/slab.h>

#define PHUANG_SUPER_MAGIC 0x9456

static int phuang_test_super(struct super_block *sb, void *data)
{
    printk(KERN_DEBUG "%s", __func__);
    return sb->s_fs_info == data;
}

static int phuang_set_super(struct super_block *sb, void *data)
{
    printk(KERN_DEBUG "%s", __func__);
    sb->s_fs_info = data;
    return set_anon_super(sb, NULL);
}

static struct kmem_cache *phuang_inode_cache;

static struct inode *phuang_alloc_inode(struct super_block *sb)
{
    struct inode *inode;
    printk(KERN_DEBUG "%s", __func__);

    inode = (struct inode *)kmem_cache_alloc(phuang_inode_cache, GFP_KERNEL);

    inode->i_mtime = inode->i_atime = inode->i_ctime = CURRENT_TIME;
    return inode;
}

static void phuang_destroy_inode(struct inode *inode)
{
    printk(KERN_DEBUG "%s", __func__);
    kmem_cache_free(phuang_inode_cache, inode);
}

static void phuang_evict_inode(struct inode *inode)
{
    printk(KERN_DEBUG "%s", __func__);

    truncate_inode_pages(&inode->i_data, 0);
    end_writeback(inode);
}

static const struct super_operations phuang_sops = {
    .alloc_inode    = phuang_alloc_inode,
    .destroy_inode  = phuang_destroy_inode,
    .drop_inode     = generic_delete_inode,
    .evict_inode    = phuang_evict_inode,
    .statfs         = simple_statfs,
};

static struct dentry *phuang_root_lookup(struct inode *dir, struct dentry *dentry, struct nameidata *nd)
{
    printk(KERN_DEBUG "%s", __func__);
    return NULL;
}

static int phuang_root_getattr(struct vfsmount *mnt, struct dentry *dentry, struct kstat *stat)
{
    printk(KERN_DEBUG "%s", __func__);
    generic_fillattr(dentry->d_inode, stat);
    stat->nlink = 2;
    return 0;
}

static const struct inode_operations phuang_inode_ops = {
    .lookup     = phuang_root_lookup,
    .getattr    = phuang_root_getattr,
};

static int phuang_readdir(struct file *filp, void *dirent, filldir_t filldir)
{
    struct inode *inode;
    unsigned int ino;
    int i;
    int ret = 0;
    
    printk(KERN_DEBUG "%s", __func__);
    inode = filp->f_path.dentry->d_inode;
    ino = inode->i_ino;
    i = filp->f_pos;

    switch(i) {
        case 0:
            if (filldir(dirent, ".", 1, i, ino, DT_DIR) < 0)
                goto out;
            i++;
            filp->f_pos++;
        case 1:
            if (filldir(dirent, "..", 2, i,
                        parent_ino(filp->f_path.dentry),
                        DT_DIR) < 0)
                goto out;
            i++;
            filp->f_pos++;
#if 0
        case 3:
            if (filldir(dirent, "phuang", 6, i, ino, DT_DIR) < 0)
                goto out;
            i++;
            filp->f_pos++;
#endif
    }
    ret = 0;
out:
    return ret;
}

static const struct file_operations phuang_dir_ops = {
    .llseek         = generic_file_llseek,
    .read           = generic_read_dir,
    .readdir        = phuang_readdir,
};

static struct inode *phuang_get_inode(struct super_block *sb)
{
    struct inode *inode;
    printk(KERN_DEBUG "%s", __func__);
    inode = iget_locked(sb, 8989);
    if (inode == NULL)
        return inode;

    if (inode->i_state & I_NEW) {
        inode->i_mtime = 
            inode->i_atime = 
            inode->i_ctime = CURRENT_TIME;

        inode->i_mode = S_IFDIR | S_IRUGO | S_IXUGO;
        inode->i_nlink = 2;
        inode->i_op = &phuang_inode_ops;
       
        if (!S_ISREG(inode->i_mode))
            inode->i_fop = &phuang_dir_ops;

        unlock_new_inode(inode);
    }
    return inode;
}

static int phuang_fill_super(struct super_block *sb)
{
    struct inode *root_inode;

    printk(KERN_DEBUG "%s", __func__);
    sb->s_flags |= MS_NODIRATIME | MS_NOSUID | MS_NOEXEC;
    sb->s_blocksize = 1024;
    sb->s_blocksize_bits = 10;
    sb->s_magic = PHUANG_SUPER_MAGIC;
    sb->s_op = &phuang_sops;
    sb->s_time_gran = 1;
    
    root_inode = phuang_get_inode(sb);

    if (root_inode == NULL)
        return -ENOMEM;

    root_inode->i_uid = 0;
    root_inode->i_gid = 0;
    sb->s_root = d_alloc_root(root_inode);
    if (sb->s_root == NULL) {
        iput(root_inode);
        return -ENOMEM;
    }

    return 0;
}

static struct dentry *phuang_mount(struct file_system_type *fs_type,
        int flags, const char *dev_name, void *data)
{
    int err;
    struct super_block *sb;

    printk(KERN_DEBUG "%s", __func__);
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
    printk(KERN_DEBUG "%s", __func__);
    kill_anon_super(sb);
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
    printk(KERN_DEBUG "%s", __func__);
    
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
    printk(KERN_DEBUG "%s", __func__);
    unregister_filesystem(&phuang_fs_type);
}

module_init(init_phuang_fs);
module_exit(exit_phuang_fs);

