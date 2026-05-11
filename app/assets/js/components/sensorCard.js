export function updateSensorCard(temperature, humidity, isRaining, gasLevel) {
    displayTemp(temperature);
    displayHumid(humidity);
    displayRain(isRaining);
    displayGas(gasLevel);
}

function displayTemp(temperature) {
    const tempElement = document.getElementById("temp-value");

    if (temperature == null) {
        tempElement.textContent = "-";
        return;
    }

    tempElement.classList.remove("value--cold", "value--warm", "value--hot");

    if (temperature < 20) {
        tempElement.classList.add("value--cold");
    } else if (temperature < 35) {
        tempElement.classList.add("value--warm");
    } else {
        tempElement.classList.add("value--hot");
    }

    tempElement.textContent = temperature.toFixed(2) + "°C";
}

function displayHumid(humidity) {
    const humidElement = document.getElementById("humid-value");

    if (humidity == null) {
        humidElement.textContent = "-";
        return;
    }

    humidElement.classList.remove("value--dry", "value--normal", "value--humid");

    if (humidity < 30) {
        humidElement.classList.add("value--dry");
    } else if (humidity <= 60) {
        humidElement.classList.add("value--normal");
    } else {
        humidElement.classList.add("value--humid");
    }

    humidElement.textContent = humidity.toFixed(2) + "%";
}

function displayRain(isRaining) {
    const rainElement = document.getElementById("rain-value");

    if (isRaining == null) {
        rainElement.textContent = "-";
        return;
    }

    rainElement.classList.remove("value--rain", "value--dry");

    if (isRaining) {
        rainElement.classList.add("value--rain");
    } else {
        rainElement.classList.add("value--dry");
    }

    let condition = isRaining ? "RAIN" : "DRY";
    rainElement.textContent = condition;
}

function displayGas(gasLevel) {
    const gasElement = document.getElementById("gas-value");

    if (gasLevel == null) {
        gasElement.textContent = "-";
        return;
    }

    gasElement.classList.remove("value--safe", "value--warning", "value--danger");

    if (gasLevel < 300) {
        gasElement.classList.add("value--safe");
    } else if (gasLevel <= 700) {
        gasElement.classList.add("value--warning");
    } else {
        gasElement.classList.add("value--danger");
    }

    gasElement.textContent = gasLevel + " ppm";
}