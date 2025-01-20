import { useCallback, useEffect, useState } from "react";
import type { Route } from "../+types/root";
import {
	getBuzzer,
	getCraddle,
	getSensorData,
	setBuzzerValue,
	setCraddleValue,
} from "~/utils/accessServerData";
import { Switch } from "~/components/ui/switch";
import { Label } from "~/components/ui/label";
import { Button } from "~/components/ui/button";
import { Card, CardHeader, CardContent } from "~/components/ui/card";
import { showNotification } from "~/lib/noti";

export function meta({}: Route.MetaArgs) {
	return [
		{ title: "Smart Baby Monitoring" },
		{ name: "description", content: "Monitor infant's condition with ease." },
	];
}
// export const path = window.location.pathname;

export default function Home() {
	const [buzzer, setBuzzer] = useState(false);
	const [temperature, setTemperature] = useState<number>(-1);
	const [humidity, setHumidity] = useState<number>(-1);
	const [mq, setMq] = useState<boolean>(false);
	const [moisture, setMoisture] = useState<boolean>(false);
	const [moistureNotification, setMoistureNotification] =
		useState<boolean>(false);
	const [mqNotification, setMqNotification] = useState<boolean>(false);
	const [craddle, setCraddle] = useState(false);
	const [path, setPath] = useState("");
	// console.log(path);

	useEffect(() => {
		setPath(window.location.hostname);

		(async () => {
			const craddleValue = await getCraddle();
			const buzzerValue = await getBuzzer();
			setCraddle(craddleValue);
			setBuzzer(buzzerValue);
		})();
	}, []);

	const handleBuzzer = async () => {
		try {
			const newBuzzerState = !buzzer;
			await setBuzzerValue(newBuzzerState);
			setBuzzer(newBuzzerState);
		} catch (error) {
			console.error("Failed to update buzzer state:", error);
			showNotification("Failed to toggle buzzer. Please try again.");
		}
	};

	const handleCraddle = async () => {
		try {
			const newCraddleState = !craddle;
			await setCraddleValue(newCraddleState);
			setCraddle(newCraddleState);
		} catch (error) {
			console.error("Failed to update craddle state:", error);
			showNotification("Failed to toggle craddle. Please try again.");
		}
	};

	useEffect(() => {
		const fetchSensorData = async () => {
			try {
				const sensorData = await getSensorData();
				setTemperature(sensorData.temperature ?? -1);
				setHumidity(sensorData.humidity ?? -1);
				setMq(sensorData.mq ?? false);
				setMoisture(sensorData.moisture ?? false);

				if (sensorData.mq && !mqNotification) {
					showNotification("The infant has soiled their diaper");
					setMqNotification(true);
				}
				if (sensorData.moisture && !moistureNotification) {
					showNotification("The infant has urinated");
					setMoistureNotification(true);
				}
			} catch (error) {
				console.error("Error fetching sensor data:", error);
				showNotification("Failed to fetch sensor data. Please try again.");
			}
		};

		fetchSensorData();
	}, [mqNotification, moistureNotification]);

	return (
		<div className="space-y-6 p-8 max-w-xl mx-auto">
			<Card>
				<CardHeader>
					<h2 className="text-lg font-semibold">Smart Baby Monitoring</h2>
				</CardHeader>
				<CardContent>
					<div className="space-y-4">
						<div>
							<Label htmlFor="buzzer" className="text-lg">
								Buzzer:
							</Label>

							<Switch
								id="buzzer"
								checked={buzzer}
								onClick={handleBuzzer}
								className="mt-2 ml-3"
							/>
						</div>
						<div>
							<Label htmlFor="craddle" className="text-lg">
								Craddle:
							</Label>
							<Switch
								id="craddle"
								checked={craddle}
								onClick={handleCraddle}
								className="mt-2 ml-3"
							/>
						</div>
						<div className="space-y-2">
							<div className="flex justify-between">
								<span>Temperature: {temperature}°C</span>
								<span>Humidity: {humidity}%</span>
							</div>
							<div className="flex justify-between">
								<span>Moisture Detected: {moisture ? "Yes" : "No"}</span>
								<span>MQ Sensor: {mq ? "Soiled" : "Clean"}</span>
							</div>
						</div>
						<Button
							variant="outline"
							onClick={() => showNotification("Settings saved successfully!")}
							className="w-full mt-4">
							Save Settings
						</Button>
					</div>
				</CardContent>
			</Card>
		</div>
	);
}
