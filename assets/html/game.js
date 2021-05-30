function setTargetColor(index, color)
{
    color_name = document.getElementById("buckets").children[color]
        .getAttribute("data-color");
    document.getElementById("targets").children[index].setAttribute(
        "data-color", color_name);
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
