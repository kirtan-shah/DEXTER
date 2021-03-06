
let taskList = []

function httpGet(theUrl) {
    $.get(theUrl, function(data) { console.log(dat) })
}

async function sendPoints(points) {
    if(points == 0) return
    httpGet("http://10.0.0.116/ledprogress")
    setTimeout(() => sendPoints(points - 1), 1000)
}

function handleComplete(index) {
    let shouldSend = !taskList[index].completed
    taskList[index].completed = !taskList[index].completed
    render()
    if(shouldSend) sendPoints(taskList[index].points)
}

function promptTask() {
    addTask(prompt("Enter task name: "), prompt("Enter points: "))
}
function addTask(name, points) {
    taskList.push({ name, points, completed: false })
    render()
}
function render() {
    $('.task-list').empty() // clear contents
    for(let i = 0; i < taskList.length; i++) {
        let task = taskList[i]
        let el = $(`
            <div class="task-container">
                <div class="task-item ${task.completed ? "task-item-complete" : ""}">
                    <input id="checkbox${i}" type='checkbox' style="margin: auto;" onclick="handleComplete(${i})" ${task.completed ? "checked" : ""} />
                    <span class='task-name'>${task.name}</span>
                    <span class='task-points'>${task.points} points</span>
                </div>
            </div>
        `)
        $('.task-list').append(el)
    }
    $('.task-list').append(`
        <div style="display: flex; align-items: center;" onclick="promptTask()">
            <div class="add-item">
                <i class="fas fa-plus" style="font-size: 1.5rem"></i>
            </div>
        </div>
    `)
}

$(document).ready(function() {
    // addTask("do stuff", 2)
    // addTask("do stuff", 1)
    addTask("Do Math HW", 2)
    addTask("Take out Trash", 1)
    addTask("Run 3 miles", 2)
    addTask("Study for Physics Test", 1)
    render()
})