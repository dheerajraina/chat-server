let ws;

var element = document.querySelector(".message-container");
element.scrollTop = element.scrollHeight;

let open = document.querySelector("#open");
let close = document.querySelector(".close");
let panel = document.querySelector(".contact-detail");
let msgSendButton = document.getElementById("send");
let msgText = document.getElementById("message-input");
let msgContainer = document.getElementById("message-container");


window.onload = () => {
        try {
                ws = new WebSocket("wss://"+"chat-server-wq7m.onrender.com"+"/ws");
                ws.onopen = () => console.log('Connected to WebSocket');

                ws.onmessage = (event) => {
                        const messageDiv = document.createElement('div');
                        messageDiv.className = 'comment other';
                        const textMsgBubble = document.createElement('div');
                        textMsgBubble.className = 'bubble';
                        textMsgBubble.textContent = JSON.parse(event.data).content;

                        messageDiv.appendChild(textMsgBubble);
                        msgContainer.appendChild(messageDiv);
                        msgText.value = '';
                        msgContainer.scrollTop = msgContainer.scrollHeight;
                };

        } catch (error) {

                console.log(error.message);
        }

};




open.onclick = function () {
        gsap.set(panel, {
                display: "block",
                x: 300
        });
        gsap.to(panel, 0.5, { x: 0 });
};

close.onclick = function () {
        gsap.to(panel, 0.5, { x: 320 });
};

let openList = document.querySelector(".open-message");
let closeList = document.querySelector(".close-list");
let list = document.querySelector(".message-list");

openList.onclick = function () {
        gsap.set(list, { display: "block", x: -300 });
        gsap.to(list, 0.5, { x: 0 });
};

closeList.onclick = function () {
        gsap.to(list, 0.5, { x: -400 });
        gsap.to(list, 0, { display: "none", delay: 1 });
};

msgSendButton.addEventListener("click", () => {
        if ((msgText.value).length > 0) {

                try {
                        ws.send(JSON.stringify({ from: "user1", to: "user2", content: msgText.value }));

                        const messageDiv = document.createElement('div');
                        messageDiv.className = 'comment me';
                        const textMsgBubble = document.createElement('div');
                        textMsgBubble.className = 'bubble';
                        textMsgBubble.textContent = msgText.value;

                        messageDiv.appendChild(textMsgBubble);
                        msgContainer.appendChild(messageDiv);
                        msgText.value = '';
                        msgContainer.scrollTop = msgContainer.scrollHeight;

                } catch (error) {

                        console.log(error.message);
                }


        }


});