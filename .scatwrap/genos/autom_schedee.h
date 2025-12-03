<!DOCTYPE html>
<html lang="en">
<head>
  <meta charset="UTF-8">
  <title>genos/autom_schedee.h</title>
</head>
<body>
<!-- BEGIN SCAT CODE -->
#ifndef&nbsp;GENOS_AUTOM_SCHEDEE_H<br>
#define&nbsp;GENOS_AUTOM_SCHEDEE_H<br>
<br>
#include&nbsp;&lt;genos/schedee.h&gt;<br>
#include&nbsp;&lt;igris/compiler.h&gt;<br>
<br>
namespace&nbsp;genos<br>
{<br>
&nbsp;&nbsp;&nbsp;&nbsp;typedef&nbsp;void&nbsp;(*autom_schedee_func_t)(void&nbsp;*priv,&nbsp;int&nbsp;*state);<br>
<br>
&nbsp;&nbsp;&nbsp;&nbsp;class&nbsp;autom_schedee&nbsp;:&nbsp;public&nbsp;schedee<br>
&nbsp;&nbsp;&nbsp;&nbsp;{<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;autom_schedee_func_t&nbsp;func&nbsp;=&nbsp;nullptr;<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;void&nbsp;*priv&nbsp;=&nbsp;nullptr;<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;int&nbsp;state&nbsp;=&nbsp;0;<br>
<br>
&nbsp;&nbsp;&nbsp;&nbsp;public:<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;~autom_schedee()&nbsp;override&nbsp;=&nbsp;default;<br>
<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;//&nbsp;autom_schedee(void&nbsp;(*destructor)(schedee&nbsp;*sched)&nbsp;=&nbsp;nullptr);<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;autom_schedee(autom_schedee_func_t&nbsp;foo,<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;void&nbsp;*privdata,<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;void&nbsp;(*destructor)(schedee&nbsp;*sched)&nbsp;=&nbsp;nullptr);<br>
<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;void&nbsp;init(autom_schedee_func_t&nbsp;foo,<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;void&nbsp;*privdata,<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;void&nbsp;(*destructor)(schedee&nbsp;*sched)&nbsp;=&nbsp;nullptr);<br>
<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;autom_schedee(const&nbsp;autom_schedee&nbsp;&amp;)&nbsp;=&nbsp;delete;<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;autom_schedee&nbsp;&amp;operator=(const&nbsp;autom_schedee&nbsp;&amp;)&nbsp;=&nbsp;delete;<br>
<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;void&nbsp;execute()&nbsp;override;<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;void&nbsp;finalize()&nbsp;override;<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;int&nbsp;displace()&nbsp;override;<br>
&nbsp;&nbsp;&nbsp;&nbsp;};<br>
<br>
&nbsp;&nbsp;&nbsp;&nbsp;autom_schedee&nbsp;*create_autom_schedee(autom_schedee_func_t&nbsp;foo,<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;void&nbsp;*privdata);<br>
<br>
}<br>
<br>
#endif<br>
<!-- END SCAT CODE -->
</body>
</html>
