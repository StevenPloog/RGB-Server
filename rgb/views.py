from django.shortcuts import render, get_object_or_404
from django.http import HttpResponse
from subprocess import call
from .models import Routine
import logging

logger = logging.getLogger(__name__)

def index(request):
    routines = Routine.objects.all()
    return render(request, 'rgb/rgb.html', {'routines':routines})

# Handle button clicks
def web_input(request):
    if request.POST['type'] == 'saved':
        return saved_input(request)
    if request.POST['type'] == 'rgb':
        return rgb_input(request)
    if request.POST['type'] == 'rgb_fade':
        return rgb_fade_input(request)

# Handle input from the RGB color picker
def rgb_input(request):
    rgb = request.POST['rgb']
    r = int(rgb[1:3], 16)
    g = int(rgb[3:5], 16)
    b = int(rgb[5:7], 16)
    logger.debug('Sending RGB value from ' + request.POST['rgb'] + ' got r/g/b: ' + str(r) + '/' + str(g) + '/' + str(b))
    rgb_args = str(r) + ',' + str(g) + ',' + str(b) + ','
    args_str = '1,' + '1,' + rgb_args
    call(['/var/www/mysite/uart', str(2+len(args_str)), args_str])

    return HttpResponse(status=200)

# Handle input from the RGB fade picker
def rgb_fade_input(request):
    rgb1 = request.POST['rgb1']
    r1 = int(rgb1[1:3], 16)
    g1 = int(rgb1[3:5], 16)
    b1 = int(rgb1[5:7], 16)
    logger.debug('Sending RGB value from ' + request.POST['rgb1'] + ' got r1/g1/b1: ' + str(r1) + '/' + str(g1) + '/' + str(b1))
    rgb1_args = '3,' + str(r1) + ',' + str(g1) + ',' + str(b1) + ','
    
    rgb2 = request.POST['rgb2']
    r2 = int(rgb2[1:3], 16)
    g2 = int(rgb2[3:5], 16)
    b2 = int(rgb2[5:7], 16)
    logger.debug('Sending RGB value from ' + request.POST['rgb2'] + ' got r2/g2/b2: ' + str(r2) + '/' + str(g2) + '/' + str(b2))
    rgb2_args = '4,' + str(r2) + ',' + str(g2) + ',' + str(b2) + ','

    time_args = '2,' + str(request.POST['time'])

    args_str = '3,' + rgb1_args + rgb2_args + time_args
    call(['/var/www/mysite/uart', str(2+len(args_str)), args_str])

    return HttpResponse(status=200)

# Handle input from buttons saved to the database
def saved_input(request):
    obj = get_object_or_404(Routine, uuid=request.POST['uuid'])
    cmd_string = ""
    cmd_string += str(len(obj.command_set.all())) + ','
    for cmd in obj.command_set.all():
        cmd_string += cmd.cmd + ','
        if cmd.cmd == '0' or cmd.cmd == '2':
            cmd_string += str(cmd.red) + ','
        elif cmd.cmd == '1' or cmd.cmd == '3' or cmd.cmd == '4':
            cmd_string += str(cmd.red) + ','
            cmd_string += str(cmd.green) + ','
            cmd_string += str(cmd.blue) + ','

    call(['/var/www/mysite/uart', str(2+len(cmd_string)), cmd_string])

    return HttpResponse(status=200)

