export function displayTemp(value) {
    const temperature = document.getElementById("temp-value");
    temperature.textContent = value.toFixed(2) + "°C";

    temperature.classList.remove("value--cold", "value--warm", "value--hot");

    if (value < 20) {
        temperature.classList.add("value--cold");
    } else if (value < 35) {
        temperature.classList.add("value--warm");
    } else {
        temperature.classList.add("value--hot");
    }
}

export function displayHumid(value) {
    const humidity = document.getElementById("humid-value");
    humidity.textContent = value.toFixed(2) + "%";

    humidity.classList.remove("value--dry", "value--normal", "value--humid");

    if (value < 30) {
        humidity.classList.add("value--dry");
    } else if (value <= 60) {
        humidity.classList.add("value--normal");
    } else {
        humidity.classList.add("value--humid");
    }
}

export function displayRain(value) {
    const rain = document.getElementById("rain-value");
    let condition = value ? "HUJAN" : "REDA";
    rain.textContent = condition;

    rain.classList.remove("value--rain", "value--dry");

    if (value) {
        rain.classList.add("value--rain");
    } else {
        rain.classList.add("value--dry");
    }
}

export function displayGas(value) {
    const gas = document.getElementById("gas-value");
    gas.textContent = value + " ppm";

    gas.classList.remove("value--safe", "value--warning", "value--danger");

    if (value < 300) {
        gas.classList.add("value--safe");
    } else if (value <= 700) {
        gas.classList.add("value--warning");
    } else {
        gas.classList.add("value--danger");
    }
}