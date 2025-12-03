<!DOCTYPE html>
<html lang="en">
<head>
  <meta charset="UTF-8">
  <title>genos/posix/fcntl.cpp</title>
</head>
<body>
<!-- BEGIN SCAT CODE -->
#include&nbsp;&lt;fcntl.h&gt;<br>
#include&nbsp;&lt;genos/resource/lookup.h&gt;<br>
#include&nbsp;&lt;genos/schedee.h&gt;<br>
<br>
int&nbsp;open(const&nbsp;char&nbsp;*path,&nbsp;int&nbsp;__oflag)<br>
{<br>
&nbsp;&nbsp;&nbsp;&nbsp;auto&nbsp;*sch&nbsp;=&nbsp;genos::current_schedee();<br>
&nbsp;&nbsp;&nbsp;&nbsp;if&nbsp;(sch&nbsp;==&nbsp;nullptr)<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;return&nbsp;-1;<br>
<br>
&nbsp;&nbsp;&nbsp;&nbsp;auto&nbsp;&amp;res&nbsp;=&nbsp;sch-&gt;resource_table();<br>
&nbsp;&nbsp;&nbsp;&nbsp;int&nbsp;fd&nbsp;=&nbsp;res.create_openres_fd();<br>
&nbsp;&nbsp;&nbsp;&nbsp;auto&nbsp;&amp;ores&nbsp;=&nbsp;res[fd];<br>
&nbsp;&nbsp;&nbsp;&nbsp;int&nbsp;sts&nbsp;=&nbsp;ores.open(path,&nbsp;__oflag);<br>
&nbsp;&nbsp;&nbsp;&nbsp;if&nbsp;(sts&nbsp;&lt;&nbsp;0)<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;return&nbsp;sts;<br>
&nbsp;&nbsp;&nbsp;&nbsp;return&nbsp;fd;<br>
}<br>
<!-- END SCAT CODE -->
</body>
</html>
