import axios from "axios";

export async function setBuzzerValue(buzzer: boolean) {
	try {
		const response = await axios.post(
			"http://localhost:8000/api/buzzer",
			{
				buzzer,
			},
			{
				headers: {
					"Content-Type": "application/json",
				},
			}
		);
		return response.data;
	} catch (error) {
		console.error(error);
		throw new Error("Failed to set buzzer");
	}
}

export async function getSensorData() {
	try {
		const response = await axios.get("http://localhost:8000/api/sensor-data");
		// console.log("first");

		// console.log("response: ", response);
		return response.data;
	} catch (error) {
		console.error("error geting sensor data: ", error);
		// throw new Error("Failed to get sensor data");
	}
}

export async function setCraddleValue(craddle: boolean) {
	try {
		const response = await axios.post(
			"http://localhost:8000/api/craddle",
			{
				craddle,
			},
			{
				headers: {
					"Content-Type": "application/json",
				},
			}
		);
		// console.log(response);
		return response.data;
	} catch (error) {
		console.error(error);
		throw new Error("Failed to set buzzer");
	}
}

export async function getBuzzer() {
	try {
		const response = await axios.get("http://localhost:8000/buzzer");
		// console.log("Buzzer response:", response.data);
		return response.data.buzzer;
	} catch (error) {
		console.error("Error getting buzzer:", error);
		throw new Error("Failed to get buzzer");
	}
}

export async function getCraddle() {
	try {
		const response = await axios.get("http://localhost:8000/craddle");
		// console.log("Craddle response:", response.data);
		return response.data.craddle;
	} catch (error) {
		console.error("Error getting craddle:", error);
		throw new Error("Failed to get craddle");
	}
}

// ---------------------------------------

// import axios from "axios";

// export async function setBuzzerValue(buzzer: boolean) {
// 	try {
// 		const response = await axios.post(
// 			"http://localhost:8000/api/buzzer",
// 			{
// 				buzzer,
// 			},
// 			{
// 				headers: {
// 					"Content-Type": "application/json", // Change to application/json for JSON payload
// 				},
// 			}
// 		);
// 		return response.data;
// 	} catch (error) {
// 		console.error("Error setting buzzer value:", error);
// 		throw new Error("Failed to set buzzer");
// 	}
// }

// export async function getSensorData() {
// 	try {
// 		const response = await axios.get("http://localhost:8000/api/sensor-data");
// 		console.log("Sensor data response:", response);
// 		return response.data;
// 	} catch (error) {
// 		console.error("Error getting sensor data:", error);
// 		throw new Error("Failed to fetch sensor data");
// 	}
// }

// export async function setCraddleValue(craddle: boolean) {
// 	try {
// 		const response = await axios.post(
// 			"http://localhost:8000/api/craddle",
// 			{
// 				craddle,
// 			},
// 			{
// 				headers: {
// 					"Content-Type": "application/json", // Use application/json for consistency
// 				},
// 			}
// 		);
// 		console.log("Craddle response:", response.data);
// 		return response.data;
// 	} catch (error) {
// 		console.error("Error setting craddle value:", error);
// 		throw new Error("Failed to set craddle");
// 	}
// }
