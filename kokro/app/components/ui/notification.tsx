import { useState, useEffect } from "react";
import { Button } from "./button";

const NotificationComponent = () => {
	const [permission, setPermission] = useState(Notification.permission);

	useEffect(() => {
		if (Notification.permission === "default") {
			Notification.requestPermission().then((permission) => {
				setPermission(permission);
			});
		}
	}, []);

	const showNotification = () => {
		if (permission === "granted") {
			new Notification("Hello, this is your notification!");
		} else {
			alert("Permission not granted for notifications");
		}
	};

	return (
		<div>
			<Button onClick={showNotification}>Show Notification</Button>
		</div>
	);
};

export default NotificationComponent;
