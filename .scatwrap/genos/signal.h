<!DOCTYPE html>
<html lang="en">
<head>
  <meta charset="UTF-8">
  <title>genos/signal.h</title>
</head>
<body>
<!-- BEGIN SCAT CODE -->
#ifndef&nbsp;GENOS_SIGNAL_H<br>
#define&nbsp;GENOS_SIGNAL_H<br>
<br>
#include&nbsp;&lt;genos/schedee.h&gt;<br>
<br>
#define&nbsp;SIGCHLD&nbsp;-33<br>
#define&nbsp;SIGINT&nbsp;-22<br>
<br>
/*namespace&nbsp;genos<br>
{<br>
&nbsp;&nbsp;&nbsp;&nbsp;static&nbsp;inline<br>
&nbsp;&nbsp;&nbsp;&nbsp;void&nbsp;raise_signal_group(int&nbsp;gid,&nbsp;int&nbsp;sig)<br>
&nbsp;&nbsp;&nbsp;&nbsp;{<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;schedee&nbsp;*&nbsp;sch;<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;dlist_for_each_entry(sch,&nbsp;&amp;schedee_list,&nbsp;schedee_list_lnk)<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;{<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;if&nbsp;(sch-&gt;gid&nbsp;==&nbsp;gid)<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;{<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;sch-&gt;signal_handler(sig);<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;}<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;}<br>
&nbsp;&nbsp;&nbsp;&nbsp;}<br>
}*/<br>
<br>
#endif<br>
<!-- END SCAT CODE -->
</body>
</html>
