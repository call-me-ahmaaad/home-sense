import { getSensorData } from "../api/sensorData.js";
import { getDeviceStatus } from "../api/deviceStatus.js";
import { updateSensorCard } from "../components/sensorCard.js";
import { ledControl } from "../components/ledControl.js";

const sensorValue = document.querySelectorAll(".dashboard-card__value");

async function updateDashboard() {
    try {
        const sensorDataResponse = await getSensorData();
        const sensorData = sensorDataResponse.data;

        const deviceStatusResponse = await getDeviceStatus();
        const deviceStatus = deviceStatusResponse.data;

        const temperature = sensorData.temperature?.temperature ?? null;
        const humidity = sensorData.humidity?.humidity ?? null;
        const isRaining = sensorData.rain_status != null ? sensorData.rain_status.is_raining === 1 : null;
        const gasLevel = sensorData.gas_level?.gas_level ?? null;

        if (deviceStatus.status === "offline" || deviceStatus === null) {
            sensorValue.forEach(value => value.textContent = "-");
        } else {
            updateSensorCard(
                temperature,
                humidity,
                isRaining,
                gasLevel
            );
        }
    } catch (error) {
        console.error("Error: " + error);
    }
}

ledControl();

setInterval(updateDashboard, 2000);