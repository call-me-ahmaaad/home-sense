export function initLedControl() {
    const ledButtons = document.querySelectorAll(".led-button");

    const ledState = {
        red: false,
        green: false,
        blue: false
    };

    ledButtons.forEach(button => {
        button.addEventListener("click", () => {
            const color = button.dataset.led;
            const indicator = button.querySelector(".led-button__indicator");
            const customColor = button.dataset.color;

            ledState[color] = !ledState[color];

            if(ledState[color]){
                indicator.classList.add("led-button__indicator--on");
            }else{
                indicator.classList.remove("led-button__indicator--on");
            }
        });
    });
}