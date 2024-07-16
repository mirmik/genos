texts = []

function make_button(text, func) {
    var button = document.createElement("button");
    button.innerHTML = text;
    button.onclick = func;
    return button;
}

function init_function() {
    var xmlHttp = new XMLHttpRequest();
    xmlHttp.open("GET", "apps_full_state.json", false); // false for synchronous request
    xmlHttp.send(null);
    response = xmlHttp.responseText
    json = JSON.parse(response);
    var divtext = document.getElementById("divtext");
    divtext.innerHTML = "";

    for (var i = 0; i < json.apps.length; i++) {
        // create div
        let index = i
        var div = document.createElement("div");

        // create label for text
        var text = document.createElement("label");
        texts.push(text);
        div.appendChild(text);

        div.appendChild(make_button("Stop", function () { stop(index); }))
        div.appendChild(make_button("Start", function () { start(index); }))
        div.appendChild(make_button("Restart", function () { restart(index); }))
        div.appendChild(make_button("stdout", function () { get_stdout(index); }))

        var command_label = document.createElement("label");
        command_label.innerHTML = json.apps[i].command;
        div.appendChild(command_label);

        divtext.appendChild(div);
    }
}

function state_update_loop() {
    setInterval(function () {
        var xmlHttp = new XMLHttpRequest();
        xmlHttp.open("GET", "apps_state.json", true); // false for synchronous request
        xmlHttp.onload = function (e) {
            response = xmlHttp.responseText
            json = JSON.parse(response);

            for (var i = 0; i < json.apps.length; i++) {
                text = texts[i];
                text.innerHTML = json.apps[i].name + ": " + json.apps[i].state;
            }
        };
        xmlHttp.send(null);
    }, 100);
}

function stop_all() {
    var xmlHttp = new XMLHttpRequest();
    xmlHttp.open("GET", "stop_all.action", false); // false for synchronous request
    xmlHttp.send(null);
}

function start_all() {
    var xmlHttp = new XMLHttpRequest();
    xmlHttp.open("GET", "start_all.action", false); // false for synchronous request
    xmlHttp.send(null);
}

function stop(i) {
    var xmlHttp = new XMLHttpRequest();
    xmlHttp.open("GET", "stop.action?index=" + i, false); // false for synchronous request
    xmlHttp.send(null);
}

function start(i) {
    var xmlHttp = new XMLHttpRequest();
    xmlHttp.open("GET", "start.action?index=" + i, false); // false for synchronous request
    xmlHttp.send(null);
}

function restart(i) {
    var xmlHttp = new XMLHttpRequest();
    xmlHttp.open("GET", "restart.action?index=" + i, false); // false for synchronous request
    xmlHttp.send(null);
}

function get_stdout(i) {
    var xmlHttp = new XMLHttpRequest();
    xmlHttp.open("GET", "get_logs.action?index=" + i, true);
    xmlHttp.onload = function (e) {
        response = xmlHttp.responseText
        json = JSON.parse(response);
        log = atob(json.stdout);
        var log_area = document.getElementById("log_area");
        log_area.innerHTML = log;
    }
    xmlHttp.send(null);
} 