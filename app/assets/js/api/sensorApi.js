export function getTempAndHumidity() {
    let temperature = Math.random() * 100;
    let humidity = Math.random() * 100;

    return { temperature, humidity };
}

export function getRainStatus() {
    let isRaining = Math.random() < 0.5;

    return isRaining;
}

export function getGasLevel() {
    let gasLevel = Math.floor(Math.random() * 1501);

    return gasLevel;
}