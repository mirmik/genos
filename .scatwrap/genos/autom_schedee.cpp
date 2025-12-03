<!DOCTYPE html>
<html lang="en">
<head>
  <meta charset="UTF-8">
  <title>genos/autom_schedee.cpp</title>
</head>
<body>
<!-- BEGIN SCAT CODE -->
#include&nbsp;&lt;genos/autom_schedee.h&gt;<br>
#include&nbsp;&lt;genos/displace.h&gt;<br>
#include&nbsp;&lt;igris/util/bug.h&gt;<br>
<br>
void&nbsp;genos::autom_schedee::execute()<br>
{<br>
&nbsp;&nbsp;&nbsp;&nbsp;_execmon.start_session();<br>
&nbsp;&nbsp;&nbsp;&nbsp;func(priv,&nbsp;&amp;state);<br>
&nbsp;&nbsp;&nbsp;&nbsp;_execmon.end_session();<br>
}<br>
<br>
void&nbsp;genos::autom_schedee::finalize()&nbsp;{}<br>
<br>
int&nbsp;genos::autom_schedee::displace()<br>
{<br>
&nbsp;&nbsp;&nbsp;&nbsp;BUG();<br>
&nbsp;&nbsp;&nbsp;&nbsp;return&nbsp;-1;<br>
}<br>
<br>
genos::autom_schedee::autom_schedee(autom_schedee_func_t&nbsp;foo,<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;void&nbsp;*privdata,<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;void&nbsp;(*destructor)(genos::schedee&nbsp;*sched))<br>
&nbsp;&nbsp;&nbsp;&nbsp;:&nbsp;schedee(destructor)<br>
{<br>
&nbsp;&nbsp;&nbsp;&nbsp;this-&gt;state&nbsp;=&nbsp;0;<br>
&nbsp;&nbsp;&nbsp;&nbsp;this-&gt;func&nbsp;=&nbsp;foo;<br>
&nbsp;&nbsp;&nbsp;&nbsp;this-&gt;priv&nbsp;=&nbsp;privdata;<br>
}<br>
<br>
genos::autom_schedee&nbsp;*genos::create_autom_schedee(autom_schedee_func_t&nbsp;foo,<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;void&nbsp;*privdata)<br>
{<br>
&nbsp;&nbsp;&nbsp;&nbsp;genos::autom_schedee&nbsp;*sched&nbsp;=&nbsp;new&nbsp;genos::autom_schedee(<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;foo,&nbsp;privdata,&nbsp;+[](genos::schedee&nbsp;*sched)&nbsp;{&nbsp;delete&nbsp;sched;&nbsp;});<br>
&nbsp;&nbsp;&nbsp;&nbsp;return&nbsp;sched;<br>
}<br>
<!-- END SCAT CODE -->
</body>
</html>
