<!DOCTYPE html>
<html lang="en">
<head>
  <meta charset="UTF-8">
  <title>genos/resource/HIIDE/mvfs.h</title>
</head>
<body>
<!-- BEGIN SCAT CODE -->
#ifndef&nbsp;GENOS_MVFS_H<br>
#define&nbsp;GENOS_MVFS_H<br>
<br>
#include&nbsp;&lt;genos/resource/namespace.h&gt;<br>
#include&nbsp;&lt;igris/compiler.h&gt;<br>
<br>
__BEGIN_DECLS<br>
<br>
int&nbsp;mvfs_lookup(struct&nbsp;file_head&nbsp;**filp,&nbsp;const&nbsp;char&nbsp;*path);<br>
int&nbsp;mvfs_open(const&nbsp;char&nbsp;*path,&nbsp;int&nbsp;mode,&nbsp;int&nbsp;flags);<br>
<br>
__END_DECLS<br>
<br>
#endif<br>
<!-- END SCAT CODE -->
</body>
</html>
