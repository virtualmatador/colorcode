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
    
        vars.style.setProperty("--correct-name", "'Yes'");
        vars.style.setProperty("--half-name", "'Color'");
        vars.style.setProperty("--wrong-name", "'No'");
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
    document.getElementById("targets").children[index].firstElementChild.setAttribute(
        "data-color", color);
}

function clearTargetColor(index)
{
    document.getElementById("targets").children[index].firstElementChild.setAttribute(
        "data-color", "empty");
}

function setTargetActive(index)
{
    document.getElementById("targets").children[index].firstElementChild.setAttribute(
        "data-active", "");
}

function clearTargetActive(index)
{
    document.getElementById("targets").children[index].firstElementChild.removeAttribute(
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
    var row = document.createElement("tr");
    for (color in colors)
    {
        let x = color;
        var td = document.createElement("td");
        var sel = document.createElement("button");
        sel.classList.add("cell");
        sel.classList.add("colored");
        sel.classList.add("column");
        sel.setAttribute("data-color", colors[color]);
        sel.onclick = function ()
        {
            CallHandler("guess", "click", y.toString() + " " + x.toString());
        }
        td.appendChild(sel);
        row.appendChild(td);
    }
    var score_count = 0;
    for (i in scores)
    {
        for (var j = 0; j < scores[i]; ++j)
        {
            let x = score_count + j;
            var td = document.createElement("td");
            var score = document.createElement("button");
            score.classList.add("score");
            score.classList.add("colored");
            score.setAttribute("data-color", ["full", "half"][i]);
            score.onclick = function ()
            {
                CallHandler("score", "click", y.toString() + " " + x.toString());
            }
            td.appendChild(score);
            row.appendChild(td);
        }
        score_count += scores[i];
    }
    for (var j = score_count; j < colors.length; ++j)
    {
        let x = j;
        var td = document.createElement("td");
        var score = document.createElement("button");
        score.classList.add("score");
        score.classList.add("colored");
        score.setAttribute("data-color", "wrong");
        score.onclick = function ()
        {
            CallHandler("score", "click", y.toString() + " " + x.toString());
        }
        td.appendChild(score);
        row.appendChild(td);
    }
    rows.insertBefore(row, rows.firstChild);
}

function gameOver(state)
{
    var banner = document.getElementById("gameover");
    if (state == 1)
    {
        banner.innerText = "You Win!";
        banner.style.backgroundColor = "green";
        banner.style.display = "block";
    }
    else if (state == 2)
    {
        banner.innerText = "Game Over!";
        banner.style.backgroundColor = "red";
        banner.style.display = "block";
    }
    else
    {
        banner.style.display = "none";
    }
}
