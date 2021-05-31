function setTargetColor(index, color)
{
    document.getElementById("targets").children[index].setAttribute(
        "data-color", color);
}

function clearTargetColor(index)
{
    document.getElementById("targets").children[index].setAttribute(
        "data-color", "white");
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
    while (rows.firstChild)
    {
        rows.removeChild(rows.firstChild);
    }
}

function addRow(colors, scores)
{
    var row = document.createElement("div");

    var cells = document.createElement("div");
    for (color in colors)
    {
        var sel = document.createElement("span");
        sel.classList.add("colored");
        sel.setAttribute("data-color", colors[color]);
        cells.appendChild(sel);
    }
    cells.classList.add("cell");
    row.appendChild(cells);

    var space = document.createElement("div");
    space.classList.add("space");
    row.appendChild(space);

    var resulrs = document.createElement("div");
    var score_count = 0;
    for (i in scores)
    {
        for (var j = 0; j < scores[i]; ++j)
        {
            var score = document.createElement("span");
            score.classList.add("colored");
            score.setAttribute("data-color", ["black", "gray"][i]);
            resulrs.appendChild(score);
        }
        score_count += scores[i];
    }
    for (var i = score_count; i < colors.length; ++i)
    {
        var score = document.createElement("span");
        score.classList.add("colored");
        score.setAttribute("data-color", "white");
        resulrs.appendChild(score);
    }
    resulrs.classList.add("score");
    row.appendChild(resulrs);

    row.classList.add("row");
    var rows = document.getElementById("rows");
    rows.insertBefore(row, rows.firstChild);
}

function gameOver(state)
{
    if (state)
    {
        document.getElementById("rows").style.backgroundColor = "cyan";
    }
    else
    {
        document.getElementById("rows").style.backgroundColor = "khaki";
    }
}
