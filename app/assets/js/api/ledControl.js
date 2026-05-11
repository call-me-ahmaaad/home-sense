export async function postLedControl(color, state) {
    try {
        const response = await fetch("http://localhost:8000/api/ControlLed.php", {
            method: "POST",
            headers: {
                "Content-Type": "application/json"
            },
            body: JSON.stringify({
                led: color,
                state: state
            })
        });

        const result = await response.json();

        return result;
    } catch (error) {
        throw new TypeError("Failed to post led control");
    }
}