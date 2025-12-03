<!DOCTYPE html>
<html lang="en">
<head>
  <meta charset="UTF-8">
  <title>genos/posix/sys/select.cpp</title>
</head>
<body>
<!-- BEGIN SCAT CODE -->
#include&nbsp;&lt;igris/util/bug.h&gt;<br>
#include&nbsp;&lt;sys/select.h&gt;<br>
<br>
int&nbsp;select(int&nbsp;nfds,<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;fd_set&nbsp;*readfds,<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;fd_set&nbsp;*writefds,<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;fd_set&nbsp;*exceptfds,<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;struct&nbsp;timeval&nbsp;*timeout)<br>
{<br>
&nbsp;&nbsp;&nbsp;&nbsp;BUG();<br>
}<br>
<!-- END SCAT CODE -->
</body>
</html>
