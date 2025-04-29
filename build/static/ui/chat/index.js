// let ws;

// var element = document.querySelector(".message-container");
// element.scrollTop = element.scrollHeight;

// let open = document.querySelector("#open");
// let close = document.querySelector(".close");
// let panel = document.querySelector(".contact-detail");
// let msgSendButton = document.getElementById("send");
let msgText = document.getElementById("input-message");
let msgContainer = document.getElementById("messages");


window.onload = () => {
        try {
                ws = new WebSocket("wss://"+"chat-server-wq7m.onrender.com"+"/ws");
                ws.onopen = () => console.log('Connected to WebSocket');

                ws.onmessage = (event) => {
                        const messageDiv = document.createElement('div');
                        messageDiv.className = 'msg-text';
                        const textMsgBubble = document.createElement('span');
                        textMsgBubble.className = 'text';
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




// open.onclick = function () {
//         gsap.set(panel, {
//                 display: "block",
//                 x: 300
//         });
//         gsap.to(panel, 0.5, { x: 0 });
// };

// close.onclick = function () {
//         gsap.to(panel, 0.5, { x: 320 });
// };

// let openList = document.querySelector(".open-message");
// let closeList = document.querySelector(".close-list");
// let list = document.querySelector(".message-list");

// openList.onclick = function () {
//         gsap.set(list, { display: "block", x: -300 });
//         gsap.to(list, 0.5, { x: 0 });
// };

// closeList.onclick = function () {
//         gsap.to(list, 0.5, { x: -400 });
//         gsap.to(list, 0, { display: "none", delay: 1 });
// };

// msgSendButton.addEventListener("click", () => {
//         if ((msgText.value).length > 0) {

//                 try {
//                         ws.send(JSON.stringify({ from: "user1", to: "user2", content: msgText.value }));

//                         const messageDiv = document.createElement('div');
//                         messageDiv.className = 'comment me';
//                         const textMsgBubble = document.createElement('div');
//                         textMsgBubble.className = 'bubble';
//                         textMsgBubble.textContent = msgText.value;

//                         messageDiv.appendChild(textMsgBubble);
//                         msgContainer.appendChild(messageDiv);
//                         msgText.value = '';
//                         msgContainer.scrollTop = msgContainer.scrollHeight;

//                 } catch (error) {

//                         console.log(error.message);
//                 }


//         }


// });



//Selector
const $ = (selector) => document.querySelector(selector);

//Chat
const chat = $('#chat');

//Options
const options = $('#options'),
        button_options = $('#button-options'),
        close_options = $('#close-options');

button_options.addEventListener('click', () => {
        options.classList.toggle('active');
});

close_options.addEventListener('click', () => {
        options.classList.remove('active');
});

//Dark Mode
const toggle = $('#input-dark');

toggle.addEventListener('change', function () {
        this.checked ? chat.classList.add('dark') : chat.classList.remove('dark');
});

//Colors
const blue = $('#color-blue'),
        red = $('#color-red'),
        green = $('#color-green'),
        purple = $('#color-purple');

blue.addEventListener('click', () => {
        delete chat.dataset.color;
});
red.addEventListener('click', () => {
        chat.dataset.color = 'red';
});
green.addEventListener('click', () => {
        chat.dataset.color = 'green';
});
purple.addEventListener('click', () => {
        chat.dataset.color = 'purple';
});

//Messages
const messages = $('#messages'),
        input = $('#input-message'),
        send = $('#send-text');

//Send messages
input.addEventListener('keypress', function (ev) {
        if (ev.key === 'Enter' && input.value) {
                ev.preventDefault();
                const msg = input.value;
                sendMessage(msg);
        };
});

send.addEventListener('click', () => {
        const msg = input.value;
        if (msg) sendMessage(msg);
});

const sendMessage = (msg) => {
        //Create message
        const block = document.createElement('div');
        block.setAttribute('class', 'msg-text owner');
        const message = document.createElement('span');
        message.setAttribute('class', 'text');
        message.innerText = String(msg);

        //Append message
        block.appendChild(message);
        messages.appendChild(block);
        messages.scrollTo(0, 99999);

        try {
                ws.send(JSON.stringify({ from: "user1", to: "user2", content: msgText.value }));


        } catch (error) {

                console.log(error.message);
        }


        //Clear input
        input.value = '';
        input.focus();

};