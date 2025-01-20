import express from "express";
import SensorData from "./sensorData.js";
import cors from "cors";
import bodyParser from "body-parser";
const sensorData = new SensorData();

const app = express();
const port = 8000;

const sensorObject = {
	craddle: false,
	buzzer: false,
};

app.use(bodyParser.json());
app.use(
	cors({
		origin: "*", // Allow all origins
		methods: "GET,POST,PUT,DELETE", // Allow specific methods
		credentials: true, // Allow cookies/credentials
	})
);

// Route to handle the POST request
app.post("/sensor-data", (req, res) => {
	// Access the parsed data in req.body
	const { temperature, humidity, mq, moisture } = req.body;
	// console.log(req.body);
	sensorData.setTemperature(temperature);
	sensorData.setHumidity(humidity);
	sensorData.setMq(mq);
	sensorData.setMoisture(moisture);

	// Log the values
	console.log(
		`Received data - Temperature: ${temperature} °C, Humidity: ${humidity} %, MQ: ${mq}, Moisture: ${moisture}`
	);

	// Respond back with a message
	res.send("Received data!");
});

app.get("/buzzer", async (req, res) => {
	if (sensorObject.buzzer) {
		res.status(200).json({ buzzer: true });
	} else {
		res.status(201).json({ buzzer: false });
	}
});

app.get("/api/sensor-data", (req, res) => {
	try {
		res.json({
			temperature: sensorData.getTemperature(),
			humidity: sensorData.getHumidity(),
			mq: sensorData.getMq(),
			moisture: sensorData.getMoisture(),
		});
	} catch (error) {
		console.error(error);
		res.status(500).send("Internal server error");
	}
});

app.post("/api/buzzer", async (req, res) => {
	try {
		const { buzzer } = req.body;
		sensorObject.buzzer = buzzer;
		// console.log(sensorObject);
		// console.log(req.body);
		console.log("Setting buzzer to " + buzzer);
		await sensorData.setBuzzer(buzzer);

		// res.json({ buzzer: await sensorData.getBuzzer() });
		res.json({ buzzer: buzzer });
	} catch (error) {
		console.error(error);
		res.status(500).send("Internal server error");
	}
});

// for servo (cradle)
app.post("/api/craddle", async (req, res) => {
	try {
		let craddleOn;
		const { craddle } = req.body;
		sensorObject.craddle = craddle;
		craddleOn = Boolean(craddle);
		console.log("Setting craddle to " + craddle);
		await sensorData.setCraddle(craddleOn);
		// console.log(await sensorData.getCraddle());
		// res.json({ craddle: craddleOn });
		res.json({ craddle: sensorObject.craddle });
	} catch (error) {
		console.error(error);
		res.status(500).send("Internal server error");
	}
});

app.get("/craddle", async (req, res) => {
	// if (await sensorData.getCraddle()) {
	if (sensorObject.craddle) {
		res.status(200).send({ craddle: true });
		return;
	}
	res.status(201).send({ craddle: false });
});

// Start the server
app.listen(port, () => {
	console.log(`Server listening at http://localhost:${port}`);
});
