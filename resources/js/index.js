const display_div = document.querySelector("#display_div");
const display_list = document.querySelector("#display_list");
const display_clock = document.querySelector("#display_clock");

const search_form = document.querySelector("#search_form");
search_form.addEventListener("click", sendData);

function startTime() {
    const today = new Date();
    let h = today.getHours();
    let m = today.getMinutes();
    let s = today.getSeconds();
    m = checkTime(m);
    s = checkTime(s);
    display_clock.innerHTML =  h + ":" + m + ":" + s;
    setTimeout(startTime, 1000);
}

function checkTime(i) {
    if (i < 10) {i = "0" + i};  // add zero in front of numbers < 10
    return i;
}

async function sendData() {
    // Associate the FormData object with the form element
    const formData = new FormData(search_form);
    const city = formData.get("id")

    if (city.length < 2) {
        return;
    }

    try {
        const response = await fetch("/api/weather?city=" + city, {
            method: "GET",
            // Set the FormData instance as the request body
            //body: formData,
        });
        const weather_report = await response.json();
    }
    
    catch (e) {
        console.error(e);
    }
}

// Take over form submission
search_form.addEventListener("submit", (event) => {
    event.preventDefault();
    sendData();
});
