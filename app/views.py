from django.shortcuts import redirect, render
from django.http import HttpResponse
from django.template import loader
from django.contrib.auth import authenticate, login
from django.contrib import messages
from django.contrib.auth.decorators import login_required
from django.contrib.auth.models import User
from .models import *
from .validation import validate
from datetime import datetime

def home(request):
    template = loader.get_template('app/posts.html')

    posts = Post.objects.all()
    context = {
        'posts':posts
    }
    return HttpResponse(template.render(context, request))

def index(request):
    return redirect('home')

def login_view(request):
    template = loader.get_template('app/login.html')

    # handle POST request
    if request.method == "POST":
        username = request.POST.get('username')
        password = request.POST.get('password')

        # check if a user with that username exists in the database
        if not User.objects.filter(username=username).exists():
            context = {'message':"Invalid username"}
            return HttpResponse(template.render(context, request))
        
        # check if the provided password is correct
        user = authenticate(username=username, password=password)
        
        if user is None:
            context = {'message':"Invalid password"}
            return HttpResponse(template.render(context, request))
        else:
            login(request, user)
            return redirect('/home/')
    
    # handle GET request - display the normal page
    context = {'message':""}
    return HttpResponse(template.render(context, request))

def register_view(request):
    template = loader.get_template('app/register.html')

    # handle POST request
    if request.method == 'POST':
        username = request.POST.get('username')
        password = request.POST.get('password')
        first_name = request.POST.get('first_name')
        last_name = request.POST.get('last_name')
        email = request.POST.get('email')

        # check if a user with that usernmae exists
        user = User.objects.filter(username=username)
        if user.exists():
            context = {'message':"Username is already taken"}
            return HttpResponse(template.render(context, request))
        
        # validate all the fields
        validationMessage = validate(username, password, email, first_name, last_name)
        if validationMessage != 0:
            context = {'message':validationMessage}
            return HttpResponse(template.render(context, request))

        # create a new user
        user = User.objects.create_user(
            username=username,
            first_name=first_name,
            last_name=last_name,
            email=email
        )
        
        # set the user password
        user.set_password(password)
        user.save()
        
        context = {'message':"Account created successfuly"}
        return HttpResponse(template.render(context, request))
    
    #handle GET request
    context = {'message':""}
    return HttpResponse(template.render(context, request))

def create_post(request):
    template = loader.get_template('app/create-post.html')

    if request.method == 'POST':
        user = request.user
        title = request.POST.get('title')
        body = request.POST.get('body')
        time = datetime.now()

        post = Post.objects.create (
            author = user,
            title = title,
            body = body,
            date = time
        )

        post.save()

        return redirect('/home/')

    return HttpResponse(template.render({},request));

def user_info(request, username):
    template = loader.get_template('app/home.html')

    try:
        user = User.objects.get(username=username)
        context = {'content': user.username}
    except User.DoesNotExist:
        context = {'content': "No such user"}

    return HttpResponse(template.render(context ,request));