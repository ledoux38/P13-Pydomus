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
from django.contrib.auth.models import User
import json
import requests

from .forms import Login, Contact
from .variables_globales import KEY, CRYPTAGES
from .utils import Crypthographie, convert_dict_to_list, convert_list_to_dict




# Create your views here.

@login_required(login_url='/domus/login')
def index(request):
    """ensures the display of the home page"""
    context = {"index":True}
    return render(request, 'domus/index.html', context)

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
    else:
        if request.user.is_authenticated:
            context = {"index":True}
            return render(request, 'domus/index.html', context)
        else:
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
    # If the method is of type POST
    if request.method == 'POST':
        user = User.objects.get(username=request.user)

        if not request.POST.get("username") == None:
            user.username = request.POST.get("username")
            user.save()
        if not request.POST.get("nom") == None:
            user.last_name = request.POST.get("nom")
            user.save()
        if not request.POST.get("prenom") == None:
            user.first_name = request.POST.get("prenom")
            user.save()
        if not request.POST.get("email") == None:
            user.email = request.POST.get("email")
            user.save()
        if not request.POST.get("mdp") == None:
            user.set_password = request.POST.get("mdp")
            user.save()
        if not request.POST.get("url") == None:
            pass

    # If the method is of type GET
    else:
        pass

    return render(request, 'domus/settings.html')

    # return render(request, 'domus/settings.html')

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
        name = request.POST.get('name')
        mail = request.POST.get('email')
        objet = request.POST.get('objet')
        textArea = request.POST.get('textArea')
        toMail = "ledoux.florian30@gmail.com"
        send_mail(objet, textArea, mail, [toMail], fail_silently=False,)

    form = Contact()
    context = {"login": form}
    return render(request, 'domus/contact.html', context)

@csrf_exempt
def update(request):
    #VARIABLES
    url = "http://pydomus.hopto.org:3000"
    r = ""
    context = {}
    param = {KEY[0]: KEY[1]}
    cryptage = Crypthographie()
    cryptage.add_key(CRYPTAGES[0][0], CRYPTAGES[0][1], CRYPTAGES[0][2])
    cryptage.add_key(CRYPTAGES[1][0], CRYPTAGES[1][1], CRYPTAGES[1][2])

    #IF REQUEST IS POST
    if request.method == 'POST':
        # recovery of the key of the request
        # for preparation of the url towards
        # the microcontroller
        # preparing the URL

        for i in request.POST.keys():
            param[i] = request.POST.get(i)

        param = convert_dict_to_list(param)
        container_crypt = []
        for i in param:
            p = cryptage.cryptage(i[0], "param")
            v = cryptage.cryptage(i[1], "valeur")
            container_crypt.append([p, v])

        param = convert_list_to_dict(container_crypt)
        print(param)

        requests.get(url, params=param)

    #ELSE REQUEST IS GET
    else:
        param = convert_dict_to_list(param)
        container_crypt = []
        for i in param:
            p = cryptage.cryptage(i[0], "param")
            v = cryptage.cryptage(i[1], "valeur")
            container_crypt.append([p, v])

        param = convert_list_to_dict(container_crypt)
        r = requests.get(url, params=param).json()
        print(r)
        dectryp_dict = {}
        for keys, values in r.items():
            dectryp_liste = []
            for i in values:
                dectryp_liste.append(cryptage.decryptage(i, "valeur"))
            dectryp_dict[cryptage.decryptage(keys, "param")] = dectryp_liste
        print(dectryp_dict)
        context = {'valeur': dectryp_dict}

    return JsonResponse(context)

def test(request):
    context = {}
    url = "http://pydomus.hopto.org:3000/"
    r = requests.get(url).json()
    print(r)
    context = {'valeur': r}
    return render(request, 'domus/test.html', context)
