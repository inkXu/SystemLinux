function verify() {
    let name=document.getElementById('username').value
    let password=document.getElementById('password').value
    let forms = document.getElementById('form')
    if(name=="" || name==null || password=="" || password==null){
        alert('账号或密码不能为空!')
        forms.addEventListener('submit',function(evt){evt.preventDefault()})
    }
    else if(name=='admin'&&password=='admin'){
        location.href='./index.html'
    }
    else{
        alert('账号密码不正确!')
        forms.addEventListener('submit',function(evt){evt.preventDefault()})
    }
}