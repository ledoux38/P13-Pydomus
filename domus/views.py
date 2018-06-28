from django.contrib.auth.decorators import login_required
from django.contrib.auth import get_user_model
from django.shortcuts import render, get_object_or_404
from django.core.paginator import Paginator, PageNotAnInteger, EmptyPage
from django.db import transaction, IntegrityError
from django.http import HttpResponse
from django.contrib.auth import authenticate, login, logout

import json
import requests
from .forms import Login

# Create your views here.

@login_required(login_url='/domus/login')
def index(request):
    """ensures the display of the home page"""
    return render(request, 'domus/index.html')

def loginUser(request):
    """ensures the display of the login page
    as well as the authentication
    procedure of the user
    """
    # If the method is of type POST
    if request.method == 'POST':
        form = Login(request.POST)
        if form.is_valid():
            # Processing form data
            name = form.cleaned_data['name']
            password = form.cleaned_data['password']
            user = authenticate(username=name, password=password)
            #We check if the data is correct
            if user is not None:
                login(request, user)
                return render(request, 'domus/index.html')
    # If the method is of type GET
    form = Login()
    context = {"login": form}
    return render(request, 'domus/login.html', context)

def settings(request):
    """ensures the display of the login page
    as well as the authentication
    procedure of the user
    """
    message = "You asked for the settings page"
    return HttpResponse(message)

def controlPage(request):
    """the control page is a simple page
     for sending commands to the microcontroller
     and receiving information too."""
    if request.method == 'POST':
        valeur = request.POST.get('valeur')
        valeur = int(valeur)
        print(type(valeur))
        if valeur == 0:
            r = requests.get('http://192.168.1.22?5=0')
            return render(request, 'domus/control.html')
        elif valeur == 1:
            r = requests.get('http://192.168.1.22?5=1')
            return render(request, 'domus/control.html')
        else:
            return render(request, 'domus/control.html')


    else:
        # r = requests.get('http://192.168.1.22')
        # value = json.loads(r.text)
        # print(value)
        return render(request, 'domus/control.html')
