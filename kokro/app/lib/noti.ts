export const showNotification = async (message: string) => {
	const permission = await Notification.requestPermission();
	if (permission === "granted") {
		new Notification(message);
	} else {
		console.log("Notification permission not granted.");
	}
};
