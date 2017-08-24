var progressDialogElement = document.getElementById('progress-dialog');
var statusElement = document.getElementById('status');
var progressElement = document.getElementById('progress');
var gameElement = document.getElementById('game');

var Module = {
    preRun: [],
    postRun: [],

    print: function(text) {
        if (arguments.length > 1)
            text = Array.prototype.slice.call(arguments).join(' ');
        console.log(text);
    },

    printErr: function(text) {
        if (arguments.length > 1)
            text = Array.prototype.slice.call(arguments).join(' ');
        console.error(text);
    },

    canvas: (function() {
        var canvas = document.getElementById('canvas');
        canvas.addEventListener("webglcontextlost", function(e) {
            alert('WebGL context lost. You will need to reload the page.');
            e.preventDefault();
        }, false);
        return canvas;
    })(),

    setStatus: function(text) {
        if (!Module.setStatus.last)
            Module.setStatus.last = { time: Date.now(), text: '' };
        if (text === Module.setStatus.text)
            return;
        var m = text.match(/([^(]+)\((\d+(\.\d+)?)\/(\d+)\)/);
        var now = Date.now();
        if (m && now - Date.now() < 30)
            return; // if this is a progress update, skip it if too soon

        statusElement.innerHTML = text;

        if (m) {
            text = m[1];
            progressElement.value = this.retrievedDependencies;
            progressElement.max = this.totalDependencies;
        } else {
            progressElement.value = this.totalDependencies;
            progressElement.max = this.totalDependencies;
            if (!text) {
                gameElement.style.display = "block";
                progressDialogElement.style.display = "none";
            }
        }
    },

    totalDependencies: 0,
    retrievedDependencies: 0,
    monitorRunDependencies: function(left) {
        this.totalDependencies = Math.max(this.totalDependencies, left);
        this.retrievedDependencies = this.totalDependencies - left;
        Module.setStatus(left ? 'Preparing... (' + (this.retrievedDependencies) + '/' + this.totalDependencies + ')' : 'All downloads complete.');
    }
};

Module.setStatus('Downloading...');
window.onerror = function() {
    Module.setStatus('Exception thrown, see JavaScript console');
    Module.setStatus = function(text) {
        if (text)
            Module.printErr('[post-exception status] ' + text);
    };
};

document.onkeydown = function(e) {
    Module.ccall('user_input', 'number', ['number'],
                 [('which' in e) ? e.which : e.keyCode]);
};

var started = false;
document.getElementById('start').onclick = function(e) {
    started = !started;
    if (started)
        txt = 'stop';
    else
        txt = 'start';
    document.getElementById('start').innerHTML = txt;
};

function main_loop(timestamp) {
    if (started)
        Module.ccall('next_frame', '', [], []);
    window.requestAnimationFrame(main_loop);
}

window.requestAnimationFrame(main_loop);
