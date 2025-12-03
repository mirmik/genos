<!DOCTYPE html>
<html lang="en">
<head>
  <meta charset="UTF-8">
  <title>genos/schedee_api.h</title>
</head>
<body>
<!-- BEGIN SCAT CODE -->
#ifndef&nbsp;GENOS_SCHEDEE_API_H<br>
#define&nbsp;GENOS_SCHEDEE_API_H<br>
<br>
#include&nbsp;&lt;genos/schedee.h&gt;<br>
#include&nbsp;&lt;igris/compiler.h&gt;<br>
<br>
namespace&nbsp;genos<br>
{<br>
&nbsp;&nbsp;&nbsp;&nbsp;void&nbsp;__schedee_final(genos::schedee&nbsp;*sch);<br>
&nbsp;&nbsp;&nbsp;&nbsp;void&nbsp;current_schedee_exit();<br>
&nbsp;&nbsp;&nbsp;&nbsp;int&nbsp;current_schedee_msleep(unsigned&nbsp;int&nbsp;ms);<br>
&nbsp;&nbsp;&nbsp;&nbsp;int&nbsp;current_schedee_msleep(unsigned&nbsp;int&nbsp;ms,&nbsp;int64_t&nbsp;start);<br>
&nbsp;&nbsp;&nbsp;&nbsp;void&nbsp;current_schedee_msleep_without_displace(unsigned&nbsp;int&nbsp;ms);<br>
&nbsp;&nbsp;&nbsp;&nbsp;void&nbsp;current_schedee_msleep_without_displace(unsigned&nbsp;int&nbsp;ms,<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;int64_t&nbsp;start);<br>
&nbsp;&nbsp;&nbsp;&nbsp;int&nbsp;current_schedee_displace();<br>
<br>
&nbsp;&nbsp;&nbsp;&nbsp;int&nbsp;wait_for_avail(unsigned&nbsp;int&nbsp;fd);<br>
<br>
&nbsp;&nbsp;&nbsp;&nbsp;void&nbsp;send_signal_to_group(int&nbsp;gid,&nbsp;int&nbsp;sig);<br>
<br>
&nbsp;&nbsp;&nbsp;&nbsp;int&nbsp;kill(pid_t&nbsp;pid);<br>
}<br>
<br>
#endif<br>
<!-- END SCAT CODE -->
</body>
</html>
