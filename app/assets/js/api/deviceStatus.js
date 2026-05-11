export async function getDeviceStatus() {
    try {
        const response = await fetch("http://localhost:8000/api/DeviceStatus.php");

        const data = await response.json();

        return data;
    } catch (error) {
        throw new TypeError("Failed to fetch device status from API");
    }
}