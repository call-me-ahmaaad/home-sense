export async function getSensorData() {
    try {
        const response = await fetch("http://localhost:8000/api/SensorData.php");

        const data = await response.json();

        return data;
    } catch (error) {
        throw new TypeError("Failed to fetch sensor data from API");
    }
}