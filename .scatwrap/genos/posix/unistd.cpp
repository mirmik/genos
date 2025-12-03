<!DOCTYPE html>
<html lang="en">
<head>
  <meta charset="UTF-8">
  <title>genos/posix/unistd.cpp</title>
</head>
<body>
<!-- BEGIN SCAT CODE -->
#include&nbsp;&lt;genos/schedee.h&gt;<br>
#include&nbsp;&lt;genos/schedee_api.h&gt;<br>
#include&nbsp;&lt;unistd.h&gt;<br>
<br>
ssize_t&nbsp;write(int&nbsp;fd,&nbsp;const&nbsp;void&nbsp;*data,&nbsp;size_t&nbsp;size)<br>
{<br>
&nbsp;&nbsp;&nbsp;&nbsp;genos::resource_table&nbsp;&amp;resources&nbsp;=<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;genos::current_schedee()-&gt;resource_table();<br>
<br>
&nbsp;&nbsp;&nbsp;&nbsp;if&nbsp;(resources.size()&nbsp;&lt;=&nbsp;fd)<br>
&nbsp;&nbsp;&nbsp;&nbsp;{<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;return&nbsp;-1;<br>
&nbsp;&nbsp;&nbsp;&nbsp;}<br>
<br>
&nbsp;&nbsp;&nbsp;&nbsp;auto&nbsp;&amp;openr&nbsp;=&nbsp;resources[fd];<br>
&nbsp;&nbsp;&nbsp;&nbsp;auto&nbsp;res&nbsp;=&nbsp;openr.write(data,&nbsp;size);<br>
&nbsp;&nbsp;&nbsp;&nbsp;return&nbsp;res;<br>
}<br>
<br>
ssize_t&nbsp;read(int&nbsp;fd,&nbsp;void&nbsp;*data,&nbsp;size_t&nbsp;size)<br>
{<br>
&nbsp;&nbsp;&nbsp;&nbsp;genos::resource_table&nbsp;&amp;resources&nbsp;=<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;genos::current_schedee()-&gt;resource_table();<br>
<br>
&nbsp;&nbsp;&nbsp;&nbsp;if&nbsp;(resources.size()&nbsp;&lt;=&nbsp;fd)<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;return&nbsp;-1;<br>
<br>
&nbsp;&nbsp;&nbsp;&nbsp;auto&nbsp;&amp;openr&nbsp;=&nbsp;resources[fd];<br>
&nbsp;&nbsp;&nbsp;&nbsp;auto&nbsp;res&nbsp;=&nbsp;openr.read(data,&nbsp;size);<br>
&nbsp;&nbsp;&nbsp;&nbsp;return&nbsp;res;<br>
}<br>
<!-- END SCAT CODE -->
</body>
</html>
