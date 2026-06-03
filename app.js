document.addEventListener("DOMContentLoaded", function()
{
    const cipherSelect=document.getElementById("cipherSelect");
    const caesarKeys=document.getElementById("caesarKeys");
    const affineKeys=document.getElementById("affineKeys");
    const vigenereKeys=document.getElementById("vigenereKeys");
    const playfairKeys=document.getElementById("playfairKeys");
    const encryptBtn=document.getElementById("encryptBtn");
    const decryptBtn=document.getElementById("decryptBtn");
    const output=document.getElementById("output");
    const input=document.getElementById("input");
    const clearInputBtn=document.getElementById("clearInputBtn");
    const clearResultBtn=document.getElementById("clearResultBtn");
    const resetKeysBtn=document.getElementById("resetKeysBtn");
    const copyIcon=document.getElementById("copyIcon");
    function hideAllKeys()
    {
        caesarKeys.style.display="none";
        affineKeys.style.display="none";
        vigenereKeys.style.display="none";
        playfairKeys.style.display="none";
    }
    function showKeys()
    {
        hideAllKeys();
        const selected=cipherSelect.value;
        if(selected==="caesar")caesarKeys.style.display="block";
        if(selected==="affine")affineKeys.style.display="block";
        if(selected==="vigenere")vigenereKeys.style.display="block";
        if(selected==="playfair")playfairKeys.style.display="block";
    }
    showKeys();
    cipherSelect.addEventListener("change",showKeys);
    function runCipher(mode)
    {
        const selected=cipherSelect.value;
        let result="";
        if(selected==="caesar")
        {
            const shift=parseInt(document.getElementById("caesarShift").value);
            const cipher=new Module.CaesarCipher(shift);
            const text=document.getElementById("input").value||"";
            if(mode==="encrypt")result=cipher.encrypt(text);
            else result=cipher.decrypt(text);
        }
        if(selected==="affine")
        {
            const keyA=parseInt(document.getElementById("affineA").value);
            const keyB=parseInt(document.getElementById("affineB").value);
            const cipher=new Module.AffineCipher(keyA,keyB);
            const text=document.getElementById("input").value||"";
            if(mode==="encrypt")result=cipher.encrypt(text);
            else result=cipher.decrypt(text);
        }
        if(selected==="vigenere")
        {
            const key=document.getElementById("vigenereKey").value;
            const cipher=new Module.VigenereCipher(key);
            const text=document.getElementById("input").value||"";
            if(mode==="encrypt")result=cipher.encrypt(text);
            else result=cipher.decrypt(text);
        }
        if(selected==="playfair")
        {
            const key=document.getElementById("playfairKey").value;
            const cipher=new Module.PlayfairCipher(key);
            const text=document.getElementById("input").value||"";
            if(mode==="encrypt")result=cipher.encrypt(text);
            else result=cipher.decrypt(text);
        }
        output.value=result;
    }
    function initButtons()
    {
        encryptBtn.addEventListener("click",function(){runCipher("encrypt");});
        decryptBtn.addEventListener("click",function(){runCipher("decrypt");});
        clearInputBtn.addEventListener("click",function()
        {input.value="";});
        clearResultBtn.addEventListener("click",function()
        {output.value="";});
        resetKeysBtn.addEventListener("click",function()
        {
            document.getElementById("caesarShift").value="3";
            document.getElementById("affineA").value="5";
            document.getElementById("affineB").value="8";
            document.getElementById("vigenereKey").value="LEMON";
            document.getElementById("playfairKey").value="SECRETKEY";
        });
        copyIcon.addEventListener("click",function()
        {
            navigator.clipboard.writeText(output.value);
            copyIcon.innerHTML='<polyline points="20 6 9 17 4 12"></polyline>';
            copyIcon.setAttribute("viewBox","0 0 24 24");
            setTimeout(function()
            {
                copyIcon.innerHTML='<rect x="9" y="2" width="13" height="13" rx="2" ry="2"></rect><path d="M5 15H4a2 2 0 0 1-2-2V4a2 2 0 0 1 2-2h9a2 2 0 0 1 2 2v1"></path>';
            },1000);
        });
    }
    if(Module.calledRun)
    {
        initButtons();
    }
    else
    {
        Module.onRuntimeInitialized=initButtons;
    }
});