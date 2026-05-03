class TemperatureHumiditySensor {
    #temperature;
    #humidity;

    getTemperature() {
        return this.#temperature;
    }

    getHumidity() {
        return this.#humidity;
    }

    setTemperature(value) {
        if (typeof value !== 'number') {
            throw new Error("Temperature data must be a number!");
        }

        if (value < 0) {
            throw new Error("Temperature data must be a positive value!");
        }

        this.#temperature = value;
    }

    setHumidity(value) {
        if (typeof value !== 'number') {
            throw new Error("Humidity data must be a number!");
        }

        if (value < 0) {
            throw new Error("Humidity data must be a positive value!");
        }

        this.#humidity = value;
    }
}

class GasSensor {
    #gasLevel;

    getGasLevel() {
        return this.#gasLevel;
    }

    setGasLevel(value) {
        if (typeof value !== 'number') {
            throw new Error("Gas Level data must be a number!");
        }

        if (value < 0) {
            throw new Error("Gas Level data must be a positive value!");
        }

        this.#gasLevel = value;
    }
}

class RainSensor {
    #isRaining;

    getRainStats() {
        return this.#isRaining;
    }

    setRainStats(value) {
        if (typeof value !== 'boolean') {
            throw new Error("Rain Status data must be a boolean!");
        }

        this.#isRaining = value;
    }
}

export { TemperatureHumiditySensor, GasSensor, RainSensor };