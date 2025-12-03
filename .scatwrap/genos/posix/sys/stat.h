<!DOCTYPE html>
<html lang="en">
<head>
  <meta charset="UTF-8">
  <title>genos/posix/sys/stat.h</title>
</head>
<body>
<!-- BEGIN SCAT CODE -->
/**<br>
&nbsp;*&nbsp;@file<br>
&nbsp;*&nbsp;@brief<br>
&nbsp;*<br>
&nbsp;*&nbsp;@date&nbsp;07.09.2011<br>
&nbsp;*&nbsp;@author&nbsp;Anton&nbsp;Bondarev<br>
&nbsp;*/<br>
<br>
#ifndef&nbsp;COMPAT_POSIX_SYS_STAT_H_<br>
#define&nbsp;COMPAT_POSIX_SYS_STAT_H_<br>
<br>
#include&nbsp;&lt;limits.h&gt;<br>
#include&nbsp;&lt;sys/cdefs.h&gt;<br>
#include&nbsp;&lt;sys/types.h&gt;<br>
#include&nbsp;&lt;time.h&gt;<br>
__BEGIN_DECLS<br>
<br>
#define&nbsp;S_IFMT&nbsp;0170000&nbsp;&nbsp;&nbsp;/*&nbsp;File&nbsp;type&nbsp;mask&nbsp;*/<br>
#define&nbsp;S_IFPKT&nbsp;0160000&nbsp;&nbsp;/*&nbsp;Packet&nbsp;device&nbsp;*/<br>
#define&nbsp;S_IFSOCK&nbsp;0140000&nbsp;/*&nbsp;Socket&nbsp;*/<br>
#define&nbsp;S_IFLNK&nbsp;0120000&nbsp;&nbsp;/*&nbsp;Symbolic&nbsp;link&nbsp;*/<br>
#define&nbsp;S_IFREG&nbsp;0100000&nbsp;&nbsp;/*&nbsp;Regular&nbsp;file&nbsp;*/<br>
#define&nbsp;S_IFBLK&nbsp;0060000&nbsp;&nbsp;/*&nbsp;Block&nbsp;device&nbsp;*/<br>
#define&nbsp;S_IFDIR&nbsp;0040000&nbsp;&nbsp;/*&nbsp;Directory&nbsp;*/<br>
#define&nbsp;S_IFCHR&nbsp;0020000&nbsp;&nbsp;/*&nbsp;Character&nbsp;device&nbsp;*/<br>
#define&nbsp;S_IFIFO&nbsp;0010000&nbsp;&nbsp;/*&nbsp;Pipe&nbsp;*/<br>
<br>
#define&nbsp;__s_is(fmt,&nbsp;m)&nbsp;(((m)&amp;S_IFMT)&nbsp;==&nbsp;fmt)<br>
<br>
#define&nbsp;S_ISPKT(m)&nbsp;__s_is(S_IFPKT,&nbsp;m)&nbsp;&nbsp;&nbsp;/*&nbsp;Test&nbsp;for&nbsp;a&nbsp;packet&nbsp;device&nbsp;*/<br>
#define&nbsp;S_ISSOCK(m)&nbsp;__s_is(S_IFSOCK,&nbsp;m)&nbsp;/*&nbsp;Test&nbsp;for&nbsp;a&nbsp;socket.&nbsp;*/<br>
#define&nbsp;S_ISLNK(m)&nbsp;__s_is(S_IFLNK,&nbsp;m)&nbsp;&nbsp;&nbsp;/*&nbsp;Test&nbsp;for&nbsp;a&nbsp;symbolic&nbsp;link.&nbsp;*/<br>
#define&nbsp;S_ISREG(m)&nbsp;__s_is(S_IFREG,&nbsp;m)&nbsp;&nbsp;&nbsp;/*&nbsp;Test&nbsp;for&nbsp;a&nbsp;regular&nbsp;file.&nbsp;*/<br>
#define&nbsp;S_ISBLK(m)&nbsp;__s_is(S_IFBLK,&nbsp;m)&nbsp;&nbsp;&nbsp;/*&nbsp;Test&nbsp;for&nbsp;a&nbsp;block&nbsp;device.&nbsp;*/<br>
#define&nbsp;S_ISDIR(m)&nbsp;__s_is(S_IFDIR,&nbsp;m)&nbsp;&nbsp;&nbsp;/*&nbsp;Test&nbsp;for&nbsp;a&nbsp;directory.&nbsp;*/<br>
#define&nbsp;S_ISCHR(m)&nbsp;__s_is(S_IFCHR,&nbsp;m)&nbsp;&nbsp;&nbsp;/*&nbsp;Test&nbsp;for&nbsp;a&nbsp;character&nbsp;device.&nbsp;*/<br>
#define&nbsp;S_ISFIFO(m)&nbsp;__s_is(S_IFIFO,&nbsp;m)&nbsp;&nbsp;/*&nbsp;Test&nbsp;for&nbsp;a&nbsp;pipe&nbsp;or&nbsp;FIFO.&nbsp;*/<br>
<br>
/*&nbsp;Permissions:&nbsp;owner,&nbsp;group,&nbsp;other&nbsp;*/<br>
<br>
#define&nbsp;S_IRWXU&nbsp;0700&nbsp;/**&lt;&nbsp;owner:&nbsp;rwx&nbsp;---&nbsp;---&nbsp;*/<br>
#define&nbsp;S_IRUSR&nbsp;0400&nbsp;/**&lt;&nbsp;owner:&nbsp;r--&nbsp;---&nbsp;---&nbsp;*/<br>
#define&nbsp;S_IWUSR&nbsp;0200&nbsp;/**&lt;&nbsp;owner:&nbsp;-w-&nbsp;---&nbsp;---&nbsp;*/<br>
#define&nbsp;S_IXUSR&nbsp;0100&nbsp;/**&lt;&nbsp;owner:&nbsp;--x&nbsp;---&nbsp;---&nbsp;*/<br>
<br>
#define&nbsp;S_IRWXG&nbsp;0070&nbsp;/**&lt;&nbsp;group:&nbsp;---&nbsp;rwx&nbsp;---&nbsp;*/<br>
#define&nbsp;S_IRGRP&nbsp;0040&nbsp;/**&lt;&nbsp;group:&nbsp;---&nbsp;r--&nbsp;---&nbsp;*/<br>
#define&nbsp;S_IWGRP&nbsp;0020&nbsp;/**&lt;&nbsp;group:&nbsp;---&nbsp;-w-&nbsp;---&nbsp;*/<br>
#define&nbsp;S_IXGRP&nbsp;0010&nbsp;/**&lt;&nbsp;group:&nbsp;---&nbsp;--x&nbsp;---&nbsp;*/<br>
<br>
#define&nbsp;S_IRWXO&nbsp;0007&nbsp;/**&lt;&nbsp;other:&nbsp;---&nbsp;---&nbsp;rwx&nbsp;*/<br>
#define&nbsp;S_IROTH&nbsp;0004&nbsp;/**&lt;&nbsp;other:&nbsp;---&nbsp;---&nbsp;r--&nbsp;*/<br>
#define&nbsp;S_IWOTH&nbsp;0002&nbsp;/**&lt;&nbsp;other:&nbsp;---&nbsp;---&nbsp;-w-&nbsp;*/<br>
#define&nbsp;S_IXOTH&nbsp;0001&nbsp;/**&lt;&nbsp;other:&nbsp;---&nbsp;---&nbsp;--x&nbsp;*/<br>
<br>
/*&nbsp;TODO&nbsp;non-standard,&nbsp;but&nbsp;useful.&nbsp;--&nbsp;Eldar*/<br>
<br>
#define&nbsp;S_IRWXA&nbsp;0777&nbsp;/**&lt;&nbsp;&nbsp;&nbsp;all:&nbsp;rwx&nbsp;rwx&nbsp;rwx&nbsp;*/<br>
#define&nbsp;S_IRALL&nbsp;0444&nbsp;/**&lt;&nbsp;&nbsp;&nbsp;all:&nbsp;r--&nbsp;r--&nbsp;r--&nbsp;*/<br>
#define&nbsp;S_IWALL&nbsp;0222&nbsp;/**&lt;&nbsp;&nbsp;&nbsp;all:&nbsp;-w-&nbsp;-w-&nbsp;-w-&nbsp;*/<br>
#define&nbsp;S_IXALL&nbsp;0111&nbsp;/**&lt;&nbsp;&nbsp;&nbsp;all:&nbsp;--x&nbsp;--x&nbsp;--x&nbsp;*/<br>
<br>
/*&nbsp;TODO&nbsp;non-standard,&nbsp;BSD&nbsp;compat&nbsp;*/<br>
<br>
#define&nbsp;S_IREAD&nbsp;S_IRUSR&nbsp;&nbsp;/*&nbsp;Read&nbsp;permission,&nbsp;owner&nbsp;*/<br>
#define&nbsp;S_IWRITE&nbsp;S_IWUSR&nbsp;/*&nbsp;Write&nbsp;permission,&nbsp;owner&nbsp;*/<br>
#define&nbsp;S_IEXEC&nbsp;S_IXUSR&nbsp;&nbsp;/*&nbsp;Execute/search&nbsp;permission,&nbsp;owner&nbsp;*/<br>
<br>
/*&nbsp;TODO&nbsp;not&nbsp;implemented&nbsp;*/<br>
<br>
#define&nbsp;S_TYPEISMQ(buf)&nbsp;&nbsp;//&nbsp;Test&nbsp;for&nbsp;a&nbsp;message&nbsp;queue<br>
#define&nbsp;S_TYPEISSEM(buf)&nbsp;//&nbsp;Test&nbsp;for&nbsp;a&nbsp;semaphore<br>
#define&nbsp;S_TYPEISSHM(buf)&nbsp;//&nbsp;Test&nbsp;for&nbsp;a&nbsp;shared&nbsp;memory&nbsp;object<br>
<br>
#define&nbsp;S_ISUID&nbsp;0004000&nbsp;/*&nbsp;set&nbsp;user&nbsp;id&nbsp;on&nbsp;execution&nbsp;*/<br>
#define&nbsp;S_ISGID&nbsp;0002000&nbsp;/*&nbsp;set&nbsp;group&nbsp;id&nbsp;on&nbsp;execution&nbsp;*/<br>
#define&nbsp;S_ISVTX&nbsp;0001000&nbsp;/*&nbsp;sticky&nbsp;bit&nbsp;*/<br>
<br>
#define&nbsp;ALLPERMS&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;\<br>
&nbsp;&nbsp;&nbsp;&nbsp;(mode_t)(S_ISUID&nbsp;|&nbsp;S_ISGID&nbsp;|&nbsp;S_ISVTX&nbsp;|&nbsp;S_IRWXU&nbsp;|&nbsp;S_IRWXG&nbsp;|&nbsp;S_IRWXO)<br>
<br>
typedef&nbsp;struct&nbsp;stat<br>
{<br>
&nbsp;&nbsp;&nbsp;&nbsp;int&nbsp;st_dev;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;/*&nbsp;ID&nbsp;of&nbsp;device&nbsp;containing&nbsp;file&nbsp;*/<br>
&nbsp;&nbsp;&nbsp;&nbsp;int&nbsp;st_ino;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;/*&nbsp;inode&nbsp;number&nbsp;*/<br>
&nbsp;&nbsp;&nbsp;&nbsp;int&nbsp;st_mode;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;/*&nbsp;protection&nbsp;*/<br>
&nbsp;&nbsp;&nbsp;&nbsp;int&nbsp;st_nlink;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;/*&nbsp;number&nbsp;of&nbsp;hard&nbsp;links&nbsp;*/<br>
&nbsp;&nbsp;&nbsp;&nbsp;int&nbsp;st_uid;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;/*&nbsp;user&nbsp;ID&nbsp;of&nbsp;owner&nbsp;*/<br>
&nbsp;&nbsp;&nbsp;&nbsp;int&nbsp;st_gid;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;/*&nbsp;group&nbsp;ID&nbsp;of&nbsp;owner&nbsp;*/<br>
&nbsp;&nbsp;&nbsp;&nbsp;int&nbsp;st_rdev;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;/*&nbsp;device&nbsp;ID&nbsp;(if&nbsp;special&nbsp;file)&nbsp;*/<br>
&nbsp;&nbsp;&nbsp;&nbsp;size_t&nbsp;st_size;&nbsp;&nbsp;&nbsp;&nbsp;/*&nbsp;total&nbsp;size,&nbsp;in&nbsp;bytes&nbsp;*/<br>
&nbsp;&nbsp;&nbsp;&nbsp;size_t&nbsp;st_blksize;&nbsp;/*&nbsp;blocksize&nbsp;for&nbsp;file&nbsp;system&nbsp;I/O&nbsp;*/<br>
&nbsp;&nbsp;&nbsp;&nbsp;int&nbsp;st_blocks;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;/*&nbsp;number&nbsp;of&nbsp;512B&nbsp;blocks&nbsp;allocated&nbsp;*/<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;//	unsigned&nbsp;&nbsp;st_atime;&nbsp;&nbsp;&nbsp;/*&nbsp;time&nbsp;of&nbsp;last&nbsp;access&nbsp;*/<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;//	unsigned&nbsp;&nbsp;st_mtime;&nbsp;&nbsp;&nbsp;/*&nbsp;time&nbsp;of&nbsp;last&nbsp;modification&nbsp;*/<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;//	unsigned&nbsp;&nbsp;st_ctime;&nbsp;&nbsp;&nbsp;/*&nbsp;time&nbsp;of&nbsp;last&nbsp;status&nbsp;change&nbsp;*/<br>
<br>
&nbsp;&nbsp;&nbsp;&nbsp;struct&nbsp;timespec&nbsp;st_atim;&nbsp;&nbsp;&nbsp;&nbsp;/*&nbsp;time&nbsp;of&nbsp;last&nbsp;access&nbsp;*/<br>
&nbsp;&nbsp;&nbsp;&nbsp;struct&nbsp;timespec&nbsp;st_mtim;&nbsp;&nbsp;&nbsp;&nbsp;/*&nbsp;time&nbsp;of&nbsp;last&nbsp;modification&nbsp;*/<br>
&nbsp;&nbsp;&nbsp;&nbsp;struct&nbsp;timespec&nbsp;st_ctim;&nbsp;&nbsp;&nbsp;&nbsp;/*&nbsp;time&nbsp;of&nbsp;last&nbsp;status&nbsp;change&nbsp;*/<br>
#define&nbsp;st_atime&nbsp;st_atim.tv_sec&nbsp;/*&nbsp;Backward&nbsp;compatibility&nbsp;*/<br>
#define&nbsp;st_mtime&nbsp;st_mtim.tv_sec<br>
#define&nbsp;st_ctime&nbsp;st_ctim.tv_sec<br>
}&nbsp;stat_t;<br>
<br>
/*&nbsp;typedef&nbsp;struct&nbsp;statfs&nbsp;&nbsp;{&nbsp;*/<br>
/*&nbsp;	unsigned&nbsp;int&nbsp;bsize;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;/\*&nbsp;Fundamental&nbsp;file&nbsp;system&nbsp;block&nbsp;size&nbsp;*\/&nbsp;*/<br>
/*&nbsp;	unsigned&nbsp;int&nbsp;iosize;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;/\*&nbsp;Optimal&nbsp;transfer&nbsp;block&nbsp;size&nbsp;*\/&nbsp;*/<br>
/*&nbsp;	unsigned&nbsp;int&nbsp;blocks;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;/\*&nbsp;Total&nbsp;data&nbsp;blocks&nbsp;in&nbsp;file&nbsp;system&nbsp;*\/&nbsp;*/<br>
/*&nbsp;	unsigned&nbsp;int&nbsp;bfree;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;/\*&nbsp;Free&nbsp;blocks&nbsp;in&nbsp;fs&nbsp;*\/&nbsp;*/<br>
/*&nbsp;	unsigned&nbsp;int&nbsp;files;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;/\*&nbsp;Total&nbsp;file&nbsp;nodes&nbsp;in&nbsp;file&nbsp;system&nbsp;*\/&nbsp;*/<br>
/*&nbsp;	unsigned&nbsp;int&nbsp;ffree;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;/\*&nbsp;Free&nbsp;file&nbsp;nodes&nbsp;in&nbsp;fs&nbsp;*\/&nbsp;*/<br>
/*&nbsp;	unsigned&nbsp;int&nbsp;cachesize;&nbsp;&nbsp;&nbsp;&nbsp;/\*&nbsp;Cache&nbsp;buffers&nbsp;*\/&nbsp;*/<br>
/*&nbsp;	char&nbsp;fstype[NAME_MAX];&nbsp;&nbsp;&nbsp;/\*&nbsp;File&nbsp;system&nbsp;type&nbsp;name&nbsp;*\/&nbsp;*/<br>
/*&nbsp;	char&nbsp;mntto[PATH_MAX];&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;/\*&nbsp;Directory&nbsp;on&nbsp;which&nbsp;mounted&nbsp;*\/&nbsp;*/<br>
/*&nbsp;	char&nbsp;mntfrom[PATH_MAX];&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;/\*&nbsp;Mounted&nbsp;file&nbsp;system&nbsp;*\/&nbsp;*/<br>
/*&nbsp;}&nbsp;statfs_t;&nbsp;*/<br>
<br>
/**<br>
&nbsp;*&nbsp;Get&nbsp;file&nbsp;status&nbsp;(size,&nbsp;mode,&nbsp;mtime&nbsp;and&nbsp;so&nbsp;on)<br>
&nbsp;*/<br>
extern&nbsp;int&nbsp;chmod(const&nbsp;char&nbsp;*path,&nbsp;mode_t&nbsp;mode);<br>
//&nbsp;extern&nbsp;int&nbsp;&nbsp;&nbsp;&nbsp;fchmod(int,&nbsp;mode_t);<br>
extern&nbsp;int&nbsp;stat(const&nbsp;char&nbsp;*,&nbsp;struct&nbsp;stat&nbsp;*);<br>
extern&nbsp;int&nbsp;lstat(const&nbsp;char&nbsp;*,&nbsp;struct&nbsp;stat&nbsp;*);<br>
extern&nbsp;int&nbsp;fstat(int&nbsp;fd,&nbsp;struct&nbsp;stat&nbsp;*);<br>
extern&nbsp;int&nbsp;mkfifo(const&nbsp;char&nbsp;*,&nbsp;mode_t);<br>
extern&nbsp;int&nbsp;mknod(const&nbsp;char&nbsp;*,&nbsp;mode_t,&nbsp;dev_t);<br>
extern&nbsp;int&nbsp;mkdir(const&nbsp;char&nbsp;*,&nbsp;mode_t);<br>
extern&nbsp;mode_t&nbsp;umask(mode_t&nbsp;mode);<br>
<br>
/*<br>
&nbsp;*&nbsp;Special&nbsp;values&nbsp;for&nbsp;utimensat&nbsp;and&nbsp;futimens<br>
&nbsp;*/<br>
#define&nbsp;UTIME_NOW&nbsp;((1&nbsp;&lt;&lt;&nbsp;30)&nbsp;-&nbsp;1)<br>
#define&nbsp;UTIME_OMIT&nbsp;((1&nbsp;&lt;&lt;&nbsp;30)&nbsp;-&nbsp;2)<br>
<br>
__END_DECLS<br>
<br>
#endif&nbsp;/*&nbsp;COMPAT_POSIX_SYS_STAT_H_&nbsp;*/<br>
<!-- END SCAT CODE -->
</body>
</html>
