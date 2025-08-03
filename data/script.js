function toggleLED(state) {
    fetch('/led', {
        method: 'POST',
        headers: {
            'Content-Type': 'application/json'
        },
        body: JSON.stringify({ on: state })
    })
        .then(response => response.text())
        .then(text => {
            document.getElementById("status").innerText = text;
        });
}