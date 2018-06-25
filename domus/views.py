from django.shortcuts import render, get_object_or_404
from django.core.paginator import Paginator, PageNotAnInteger, EmptyPage
from django.db import transaction, IntegrityError
from django.http import HttpResponse
# Create your views here.

def index(request):
    """ensures the display of the home page"""
    return render(request, 'domus/index.html')

def login(request):
    """ensures the display of the login page
    as well as the authentication
    procedure of the user
    """
    return render(request, 'domus/login.html')

def settings(request):
    """ensures the display of the login page
    as well as the authentication
    procedure of the user
    """
    message = "You asked for the settings page"
    return HttpResponse(message)
