from django.conf.urls import url

from . import views

urlpatterns = [
    url(r'^$', views.index),
    url(r'^login/$', views.loginUser, name='loginUser'),
    url(r'^settings/$', views.settings, name='settings'),
]
