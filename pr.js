// Real-time Water Level Update
const slider = document.getElementById("water-level-slider");
const waterLevelText = document.getElementById("water-level");

slider.addEventListener("input", function() {
    waterLevelText.textContent = `Current Water Level: ${slider.value}%`;
});

// Real-time Prediction (Mock Prediction for demonstration)
const predictButton = document.getElementById("predict-button");
const predictionResult = document.getElementById("prediction-result");

predictButton.addEventListener("click", function() {
    // Mock prediction logic (can be replaced with actual ML model/API)
    const prediction = (Math.random() * 100).toFixed(2); // Random prediction between 0-100%
    predictionResult.textContent = `Predicted Water Usage for Next Month: ${prediction}%`;
});

// Automatic Motor Control
const motorStatus = document.getElementById("motor-status");
const toggleMotorButton = document.getElementById("toggle-motor-button");

let motorOn = false;

toggleMotorButton.addEventListener("click", function() {
    motorOn = !motorOn;
    if (motorOn) {
        motorStatus.textContent = "Motor Status: On";
        toggleMotorButton.textContent = "Turn Motor Off";
    } else {
        motorStatus.textContent = "Motor Status: Off";
        toggleMotorButton.textContent = "Turn Motor On";
    }
});
