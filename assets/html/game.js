function setText(state)
{
    vars = document.querySelector(":root");
    if (state)
    {
        vars.style.setProperty("--white-name", "'White'");
        vars.style.setProperty("--yellow-name", "'Yellow'");
        vars.style.setProperty("--orange-name", "'Orange'");
        vars.style.setProperty("--red-name", "'Red'");
        vars.style.setProperty("--green-name", "'Green'");
        vars.style.setProperty("--blue-name", "'Blue'");
        vars.style.setProperty("--empty-name", "'Empty'");
    
        vars.style.setProperty("--correct-name", "'Black'");
        vars.style.setProperty("--half-name", "'Gray'");
        vars.style.setProperty("--wrong-name", "'\u2003'");
    }
    else
    {
        vars.style.setProperty("--white-name", "");
        vars.style.setProperty("--yellow-name", "");
        vars.style.setProperty("--orange-name", "");
        vars.style.setProperty("--red-name", "");
        vars.style.setProperty("--green-name", "");
        vars.style.setProperty("--blue-name", "");
    
        vars.style.setProperty("--correct-name", "");
        vars.style.setProperty("--half-name", "");
        vars.style.setProperty("--wrong-name", "");
        vars.style.setProperty("--empty-name", "");
    }
}

function setTargetColor(index, color)
{
    document.getElementById("targets").children[index].setAttribute(
        "data-color", color);
}

function clearTargetColor(index)
{
    document.getElementById("targets").children[index].setAttribute(
        "data-color", "empty");
}

function setTargetActive(index)
{
    document.getElementById("targets").children[index].setAttribute(
        "data-active", "");
}

function clearTargetActive(index)
{
    document.getElementById("targets").children[index].removeAttribute(
        "data-active");
}

function bucket(index)
{
    CallHandler("bucket", "click", index.toString());
}

function target(index)
{
    CallHandler("target", "click", index.toString());
}

function newGame()
{
    CallHandler("game", "reset", "");
}

function stop()
{
    CallHandler("game", "stop", "");
}

function giveUp()
{
    CallHandler("game", "giveup", "");
}

function go()
{
    CallHandler("game", "step", "");
}

function clearRows()
{
    var rows = document.getElementById("rows");
    while (rows.firstElementChild)
    {
        rows.removeChild(rows.firstElementChild);
    }
}

function addRow(colors, scores)
{
    var rows = document.getElementById("rows");
    let y = rows.children.length;
    var row = document.createElement("div");
    row.classList.add("row");
    var left_col = document.createElement("div");
    left_col.classList.add("left-col");
    var index = document.createElement("button");
    index.onclick = function ()
    {
        CallHandler("index", "click", y.toString());
    }
    index.classList.add("index");
    index.innerText = y + 1;
    left_col.appendChild(index);
    for (color in colors)
    {
        let x = color;
        var sel = document.createElement("button");
        sel.classList.add("cell");
        sel.classList.add("column");
        sel.classList.add("colored");
        sel.setAttribute("data-color", colors[color]);
        sel.onclick = function ()
        {
            CallHandler("guess", "click", y.toString() + " " + x.toString());
        }
        left_col.appendChild(sel);
    }
    row.appendChild(left_col);
    var right_col = document.createElement("div");
    right_col.classList.add("right-col");
    var score_count = 0;
    for (i in scores)
    {
        for (var j = 0; j < scores[i]; ++j)
        {
            let x = score_count + j;
            var score = document.createElement("button");
            score.classList.add("score");
            score.classList.add("colored");
            score.setAttribute("data-color", ["full", "half"][i]);
            score.onclick = function ()
            {
                CallHandler("score", "click", y.toString() + " " + x.toString());
            }
            right_col.appendChild(score);
        }
        score_count += scores[i];
    }
    for (var j = score_count; j < colors.length; ++j)
    {
        let x = j;
        var score = document.createElement("button");
        score.classList.add("score");
        score.classList.add("colored");
        score.setAttribute("data-color", "wrong");
        score.onclick = function ()
        {
            CallHandler("score", "click", y.toString() + " " + x.toString());
        }
        right_col.appendChild(score);
    }
    row.appendChild(right_col);
    rows.insertBefore(row, rows.firstChild);
    row.scrollIntoView(false);
}

function gameOver(state)
{
    var banner = document.getElementById("gameover");
    banner.setAttribute("data-show", state);
    switch(state)
    {
        case 0:
            banner.textContent = "";
        break;
        case 1:
            banner.textContent = "You Win!";
        break;
        case 2:
            banner.textContent = "Game Over!";
        break;
    }
}

function more(hide)
{
    var options = document.getElementById("options");
    if (hide || options.style.display == "block")
    {
        options.style.display = "none";
    }
    else
    {
        options.style.display = "block";
    }
}

window.onclick = function(event)
{
    if (event.target != document.getElementById("more"))
    {
        more(true);
    }
}
