const slider1 = document.getElementById("slider1");
const slider2 = document.getElementById("slider2");

const ch1Display = document.getElementById("ch1");
const ch2Display = document.getElementById("ch2");

let localCh1 = null;
let localCh2 = null;
let userIsDragging = false;

// Zeigt aktuelle Spannungen im UI an
function updateDisplay() {
    ch1Display.textContent = (slider1.value / 10).toFixed(1);
    ch2Display.textContent = (slider2.value / 10).toFixed(1);
}

// Sende aktuelle Slider-Werte per POST
function sendVoltage() {
    const data = {
        voltageCh1: parseFloat((slider1.value / 10).toFixed(1)),
        voltageCh2: parseFloat((slider2.value / 10).toFixed(1))
    };

    localCh1 = data.voltageCh1;
    localCh2 = data.voltageCh2;

    fetch("/voltage", {
        method: "POST",
        headers: { "Content-Type": "application/json" },
        body: JSON.stringify(data),
    })
        .then((res) => {
            if (!res.ok) throw new Error("POST failed");
            return res.text();
        })
        .then((msg) => {
            console.log("ESP antwortet:", msg);
        })
        .catch((err) => {
            console.error("Fehler beim Senden:", err);
        });
}

// Regelmäßiges Abrufen der Werte vom ESP
function pollVoltageFromESP() {
    fetch("/voltage")
        .then((response) => response.json())
        .then((data) => {
            const ch1 = parseFloat(data.voltageCh1 || 0);
            const ch2 = parseFloat(data.voltageCh2 || 0);

            // Nur aktualisieren, wenn sich die Werte ändern und der User nicht gerade schiebt
            if (!userIsDragging && (ch1 !== localCh1 || ch2 !== localCh2)) {
                localCh1 = ch1;
                localCh2 = ch2;

                slider1.value = ch1 * 10;
                slider2.value = ch2 * 10;
                updateDisplay();
            }
        })
        .catch((err) => {
            console.error("Fehler beim Abrufen:", err);
        });
}

// User-Interaktion
slider1.addEventListener("input", () => {
    updateDisplay();
    sendVoltage();
});

slider2.addEventListener("input", () => {
    updateDisplay();
    sendVoltage();
});

// Optional: userIsDragging blockieren, wenn gedrückt
slider1.addEventListener("mousedown", () => userIsDragging = true);
slider2.addEventListener("mousedown", () => userIsDragging = true);
slider1.addEventListener("mouseup", () => userIsDragging = false);
slider2.addEventListener("mouseup", () => userIsDragging = false);

// Touch-Events für Mobilgeräte
slider1.addEventListener("touchstart", () => userIsDragging = true);
slider2.addEventListener("touchstart", () => userIsDragging = true);
slider1.addEventListener("touchend", () => userIsDragging = false);
slider2.addEventListener("touchend", () => userIsDragging = false);

// Initialer Abruf
pollVoltageFromESP();

// Intervall alle 500ms
setInterval(pollVoltageFromESP, 500);
