<!DOCTYPE html>
<html>
<head>
    <meta charset="UTF-8">
    <link rel="stylesheet" type="text/css" href="wasm-tris.css">
</head>
<body>

    <div id="progress-dialog">
        <progress value="0" max="100" id="progress"></progress>
        <div id="status">Downloading...</div>
    </div>

    <div id="game">
        <canvas width="200px" height="400px" class="emscripten"
                id="canvas" oncontextmenu="event.preventDefault()"></canvas>
        <button id="start">start</button>
    </div>

    <script type='text/javascript' src='wasm-tris-main.js'></script>
    {{{ SCRIPT }}}
</body>
</html>
