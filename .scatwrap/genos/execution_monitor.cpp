<!DOCTYPE html>
<html lang="en">
<head>
  <meta charset="UTF-8">
  <title>genos/execution_monitor.cpp</title>
</head>
<body>
<!-- BEGIN SCAT CODE -->
#include&nbsp;&lt;genos/execution_monitor.h&gt;<br>
<br>
INIT_PRIORITY(101)<br>
igris::dlist&lt;genos::execution_monitor,&nbsp;&amp;genos::execution_monitor::_lnk&gt;<br>
&nbsp;&nbsp;&nbsp;&nbsp;genos::execution_monitor_manager::_list;<br>
<br>
genos::execution_monitor::execution_monitor()<br>
{<br>
&nbsp;&nbsp;&nbsp;&nbsp;genos::execution_monitor_manager::register_controller(this);<br>
}<br>
<br>
genos::execution_monitor::execution_monitor(const&nbsp;char&nbsp;*mnemo)<br>
{<br>
&nbsp;&nbsp;&nbsp;&nbsp;_mnemo&nbsp;=&nbsp;mnemo;<br>
&nbsp;&nbsp;&nbsp;&nbsp;genos::execution_monitor_manager::register_controller(this);<br>
}<br>
<br>
std::string&nbsp;genos::execution_monitor_manager::summary()<br>
{<br>
&nbsp;&nbsp;&nbsp;&nbsp;std::string&nbsp;str;<br>
&nbsp;&nbsp;&nbsp;&nbsp;str&nbsp;+=&nbsp;&quot;Execution&nbsp;monitor&nbsp;summary\r\n&quot;;<br>
&nbsp;&nbsp;&nbsp;&nbsp;str&nbsp;+=&nbsp;&quot;count&nbsp;of&nbsp;monitored&nbsp;executions:&nbsp;&quot;;<br>
&nbsp;&nbsp;&nbsp;&nbsp;str&nbsp;+=&nbsp;std::to_string(_list.size());<br>
&nbsp;&nbsp;&nbsp;&nbsp;str&nbsp;+=&nbsp;&quot;\r\n&quot;;<br>
&nbsp;&nbsp;&nbsp;&nbsp;for&nbsp;(auto&nbsp;&amp;ctrl&nbsp;:&nbsp;_list)<br>
&nbsp;&nbsp;&nbsp;&nbsp;{<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;str&nbsp;+=&nbsp;ctrl.mnemo();<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;str&nbsp;+=&nbsp;&quot;&nbsp;sum:&quot;;<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;str&nbsp;+=&nbsp;std::to_string(ctrl.summary_time());<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;str&nbsp;+=&nbsp;&quot;&nbsp;cnt:&quot;;<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;str&nbsp;+=&nbsp;std::to_string(ctrl.summary_count());<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;str&nbsp;+=&nbsp;&quot;&nbsp;last:&quot;;<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;str&nbsp;+=&nbsp;std::to_string(ctrl.last_session_time());<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;str&nbsp;+=&nbsp;&quot;\r\n&quot;;<br>
&nbsp;&nbsp;&nbsp;&nbsp;}<br>
&nbsp;&nbsp;&nbsp;&nbsp;return&nbsp;str;<br>
}<br>
<!-- END SCAT CODE -->
</body>
</html>
