class SensorData {
	constructor() {
		this.temperature = 0;
		this.humidity = 0;
		this.mq = false;
		this.moisture = false;
		this.buzzer = false;
		this.craddle = false;
	}

	setTemperature(temperature) {
		this.temperature = temperature;
	}

	getTemperature() {
		return this.temperature;
	}

	setHumidity(humidity) {
		this.humidity = humidity;
	}

	getHumidity() {
		return this.humidity;
	}

	setMq(mq) {
		this.mq = mq;
	}

	getMq() {
		return this.mq;
	}

	async setBuzzer(buzzer) {
		this.buzzer = buzzer;
	}

	async getBuzzer() {
		return this.buzzer;
	}

	setMoisture(moisture) {
		this.moisture = moisture;
	}

	getMoisture() {
		return this.moisture;
	}

	async setCraddle(craddle) {
		this.craddle = craddle;
	}
	async getCraddle() {
		return this.craddle;
	}
}

export default SensorData;
