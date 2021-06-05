function play()
{
    CallHandler("play", "click", "");
}

function reset()
{
    CallHandler("reset", "click", "");
}

function setText(state)
{
    document.getElementById("text").checked = state;
}

function setSound(state)
{
    document.getElementById("sound").checked = state;
}

function text()
{
    CallHandler("text", "click", document.getElementById("text").checked.toString());
}

function sound()
{
    CallHandler("sound", "click", document.getElementById("sound").checked.toString());
}
