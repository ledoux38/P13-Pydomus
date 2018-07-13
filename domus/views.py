from django.contrib.auth.decorators import login_required
from django.contrib.auth import get_user_model
from django.shortcuts import render, get_object_or_404
from django.core.paginator import Paginator, PageNotAnInteger, EmptyPage
from django.db import transaction, IntegrityError
from django.http import HttpResponse
from django.contrib.auth import authenticate, login, logout
from django.http import JsonResponse
from django.views.decorators.csrf import csrf_exempt
from django.core.mail import send_mail

import json
import requests

from .forms import Login, Contact
from .variables_globales import KEY




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

def logoutUser(request):
    """function used to disconnect users"""
    logout(request)
    form = Login()
    context = {"login": form}
    return render(request, 'domus/login.html', context)

@login_required(login_url='/domus/login')
def settings(request):
    """ensures the display of the login page
    as well as the authentication
    procedure of the user
    """
    context = {}
    return render(request, 'domus/settings.html', context)

def mentionLegales(request):
    """
    Function that ensures
    the display of the legal notice page
    """
    context = {}
    return render(request, 'domus/mention_legales.html', context)

def contact(request):
    """
    Function that ensures
    the display of the contact page
    """
    # If the method is of type POST
    if request.method == 'POST':
        f = Contact(request.POST)
        name = f['name']
        mail = f['email']
        objet = f['objet']
        textArea = f['textArea']
        toMail = "ledoux.florian30@gmail.com"
        send_mail(objet, textArea, mail, [toMail], fail_silently=False,)

    form = Contact()
    context = {"login": form}
    return render(request, 'domus/contact.html', context)

@csrf_exempt
def update(request):
    #VARIABLES
    url = "http://192.168.1.22"
    r = ""
    context = {}
    list_keys = []

    #IF REQUEST IS POST
    if request.method == 'POST':

        # recovery of the key of the request
        # for preparation of the url towards
        # the microcontroller

        for cle in request.POST.keys():
            list_keys.append(cle)

        # preparing the URL

        param = list_keys[0]
        value = request.POST[list_keys[0]]
        url_get = url + "?{}={}&{}={}".format(KEY[0], KEY[1], param, value)
        requests.get(url_get)

    #ELSE REQUEST IS GET
    else:

        url_get = url + "?{}={}".format(KEY[0], KEY[1])
        r = requests.get(url_get).json()

        context = {'valeur': r}


    return JsonResponse(context)
