<!DOCTYPE html>
<html lang="en">
<head>
  <meta charset="UTF-8">
  <title>genos/shellutil.cpp</title>
</head>
<body>
<!-- BEGIN SCAT CODE -->
#include&nbsp;&lt;genos/schedee.h&gt;<br>
#include&nbsp;&lt;genos/schedee_api.h&gt;<br>
#include&nbsp;&lt;genos/shellutil.h&gt;<br>
#include&nbsp;&lt;igris/time/systime.h&gt;<br>
#include&nbsp;&lt;nos/print.h&gt;<br>
<br>
int&nbsp;info_cmd(const&nbsp;nos::argv&nbsp;&amp;args,&nbsp;nos::ostream&nbsp;&amp;os)<br>
{<br>
&nbsp;&nbsp;&nbsp;&nbsp;for&nbsp;(auto&nbsp;&amp;sch&nbsp;:&nbsp;genos::schedee_list)<br>
&nbsp;&nbsp;&nbsp;&nbsp;{<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;nos::print_to(os,&nbsp;&quot;&nbsp;mnemo:&nbsp;&quot;);<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;nos::print_to(os,&nbsp;sch.mnemo());<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;nos::print_to(os,&nbsp;&quot;&nbsp;pid:&nbsp;&quot;);<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;nos::print_to(os,&nbsp;std::to_string(sch.pid));<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;nos::print_to(os,&nbsp;&quot;&nbsp;state:&nbsp;&quot;);<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;nos::print_to(os,&nbsp;schedee_state_to_sting(sch.sch_state));<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;nos::print_to(os,&nbsp;&quot;&nbsp;prio:&nbsp;&quot;);<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;nos::print_to(os,&nbsp;std::to_string(sch.prio));<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;nos::print_to(os,&nbsp;&quot;&nbsp;counter:&nbsp;&quot;);<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;nos::print_to(os,&nbsp;std::to_string(sch._execmon.summary_count()));<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;nos::println_to(os);<br>
&nbsp;&nbsp;&nbsp;&nbsp;}<br>
&nbsp;&nbsp;&nbsp;&nbsp;return&nbsp;0;<br>
}<br>
<br>
int&nbsp;time_cmd(const&nbsp;nos::argv&nbsp;&amp;args,&nbsp;nos::ostream&nbsp;&amp;os)<br>
{<br>
&nbsp;&nbsp;&nbsp;&nbsp;uint64_t&nbsp;time&nbsp;=&nbsp;igris::micros();<br>
&nbsp;&nbsp;&nbsp;&nbsp;nos::println_to(os,&nbsp;&quot;time:&nbsp;&quot;,&nbsp;time);<br>
&nbsp;&nbsp;&nbsp;&nbsp;return&nbsp;0;<br>
}<br>
<br>
int&nbsp;execmon_cmd(const&nbsp;nos::argv&nbsp;&amp;args,&nbsp;nos::ostream&nbsp;&amp;os)<br>
{<br>
&nbsp;&nbsp;&nbsp;&nbsp;auto&nbsp;str&nbsp;=&nbsp;genos::execution_monitor_manager::summary();<br>
&nbsp;&nbsp;&nbsp;&nbsp;nos::print_to(os,&nbsp;str);<br>
&nbsp;&nbsp;&nbsp;&nbsp;return&nbsp;0;<br>
}<br>
<br>
nos::executor&nbsp;genos::schedee_manager_executor{<br>
&nbsp;&nbsp;&nbsp;&nbsp;{nos::command{&quot;ps&quot;,&nbsp;&quot;proccess&nbsp;information&quot;,&nbsp;info_cmd},<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;nos::command{&quot;time&quot;,&nbsp;&quot;time&nbsp;information&quot;,&nbsp;time_cmd},<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;nos::command{&quot;execmon&quot;,&nbsp;&quot;execution&nbsp;monitor&nbsp;information&quot;,&nbsp;execmon_cmd}}};<br>
<!-- END SCAT CODE -->
</body>
</html>
