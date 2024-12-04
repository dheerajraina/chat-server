const signUpBUtton = document.getElementById("signUp");
const signInBUtton = document.getElementById("signIn");
const container = document.getElementById("container");
const createAccountButton = document.getElementById("createAccount");

// switch between login and signup

signUpBUtton.addEventListener("click", () => {
        container.classList.add("right-panel-active");
});

signInBUtton.addEventListener("click", () => [
        container.classList.remove("right-panel-active")
]);

createAccountButton.addEventListener("click", async () => {

        console.log("create Account");
        const username = document.getElementById("createAccount_username").value;
        const password = document.getElementById("createAccount_password").value;
        const inputData = { "username": username, "password": password };
        console.log(inputData);
        try {

                const response = await fetch("http://localhost:8080/register", {
                        method: "POST",
                        headers: {
                                'Content-Type': 'application/json',
                        },
                        body: JSON.stringify(inputData)
                });
                if (!response.ok) {
                        const errorText = await response.text();
                        throw new Error("Server Error:" + errorText);
                }
                const data = await response.text();
                console.log(data);
        } catch (error) {
                console.log(error.message);

        }



});
