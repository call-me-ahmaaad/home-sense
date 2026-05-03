import { getTempAndHumidity, getRainStatus, getGasLevel } from "./api/sensorApi.js";
import { displayGas, displayRain, displayTemp, displayHumid } from "./components/sensorCard.js";
import { TemperatureHumiditySensor, GasSensor, RainSensor } from "./models/sensorData.js";
import { initLedControl } from "./components/ledControl.js";

initLedControl();

function updateDashboard() {
    const { temperature, humidity } = getTempAndHumidity();
    const isRaining = getRainStatus();
    const gasLevel = getGasLevel();

    const tempHumid = new TemperatureHumiditySensor();
    const rain = new RainSensor();
    const gas = new GasSensor();

    try {
        tempHumid.setTemperature(temperature);
        tempHumid.setHumidity(humidity);
        rain.setRainStats(isRaining);
        gas.setGasLevel(gasLevel);

        displayTemp(tempHumid.getTemperature());
        displayHumid(tempHumid.getHumidity());
        displayRain(rain.getRainStats());
        displayGas(gas.getGasLevel());
    } catch (error) {
        console.error(error);
    }
}

setInterval(updateDashboard, 1000);