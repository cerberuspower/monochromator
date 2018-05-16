import os
os.environ['KIVY_IMAGE'] = 'pil,sdl2'
import serial
import threading
import re
import time
from serial.tools import list_ports
from uart import uart
from kivy.uix.gridlayout import GridLayout
from kivy.uix.widget import Widget
from kivy.uix.boxlayout import BoxLayout
from kivy.uix.anchorlayout import AnchorLayout
from kivy.uix.floatlayout import FloatLayout
from kivy.uix.button import Button
from kivy.app import App
from kivy.uix.textinput import TextInput
from kivy.uix.label import Label
from kivy.uix.slider import Slider
from kivy.uix.dropdown import DropDown
from kivy.graphics import Color, Rectangle


class ColorLabel(Label):
    def on_size(self, *args):
        self.canvas.before.clear()
        with self.canvas.before:
            Color(1, 1, 1, 0.7)
            Rectangle(pos=self.pos, size=self.size)

class FloatInput(TextInput):
    pat = re.compile('[^0-9]')
    def insert_text(self, substring, from_undo=False):
        pat = self.pat
        if '.' in self.text:
            s = re.sub(pat, '', substring)
        else:
            s = '.'.join([re.sub(pat, '', s) for s in substring.split('.', 1)])
        return super(FloatInput, self).insert_text(s, from_undo=from_undo)

class MyApp(App):
    #------------------GLOBALS---------------------

    #--------SET Globals startup variables------------
    connection_flag = False
    slider_value = 0
    kill_event = threading.Event()
    #----------FUNCTIONS-------------
    def read(self,kill_event, x):
        self.kill_event.clear()
        print('reading thread run')
        tmp = ''
        raw = []
        while not self.kill_event.is_set():
            buf = self.ser.read_byte()
            if type(buf) is int:
                if buf != ord('\n') and buf != ord('\r'):
                    tmp += chr(buf)
                    raw.append(buf)
                elif len(raw) > 0:
                    print(tmp)
                    tmp = ''
                    raw = []
            else:
                if tmp != '' and tmp != '\n' and tmp != '\r' and len(raw) > 0:
                    print(tmp)
                    tmp = ''
                    raw = []
        print("kill thread")

    def COM_select(self,x):
        self.demand_close()
        print(x)
        self.kill_event.clear()
        self.ser = uart(baudrate=9600, name=x)
        self.connection_flag=True
        thread = threading.Thread(target=self.read, args=(self.kill_event, "task"))
        thread.start()
    def demand_close(self,*instance):
        if(self.connection_flag==True):
            self.kill_event.set()
            time.sleep(0.02)
            self.ser.close_connection()
            self.connection_flag=False
    #--------------------------------
    def on_text(self,instance, value):
        self.wavelength_value=value
    def data_send(self,instnace):
        if(self.connection_flag==True):
            if(self.wavelength_value):
                value=int(float(self.wavelength_value)*1000)
                self.ser.writeword("xgo_to%06d"%value)

    def calibration(self,*instance):
        if(self.connection_flag==True):
            self.ser.writeword("xaucal000000")
        else:
            print("\n\rCan't send calibration request: no connection open\n\r")
    def OnGetWavelengthValue(self,instance):
        if(self.connection_flag==True):
            self.ser.writeword("xgetpo000000")

    def Go_Up(self,instance):
        if(self.connection_flag==True):
            if(self.slider_value==1):
                self.ser.writeword("xgo_up001000")
            elif(self.slider_value==2):
                self.ser.writeword("xgo_up000100")
            elif(self.slider_value==3):
                self.ser.writeword("xgo_up000010")
            elif(self.slider_value==4):
                self.ser.writeword("xgo_up000001")
            else:
                self.ser.writeword("xgo_up001000")
    def Go_Down(self,instance):
        if(self.connection_flag==True):
            if(self.slider_value==1):
                self.ser.writeword("xgo_dw001000")
            elif(self.slider_value==2):
                self.ser.writeword("xgo_dw000100")
            elif(self.slider_value==3):
                self.ser.writeword("xgo_dw000010")
            elif(self.slider_value==4):
                self.ser.writeword("xgo_dw000001")
            else:
                self.ser.writeword("xgo_dw001000")

    #--------------------------------------
    def build(self):
        #--------------------------------
        dropdown = DropDown()
        for port in list_ports.comports():
            btn = Button(text='%s' % (port.description), size_hint_y=None, height=44, width=150)
            btn.bind(on_release=lambda btn: dropdown.select(btn.text))
            dropdown.add_widget(btn)
        dropdown.bind(on_select=lambda instance, x: setattr(mainbutton, 'text', x))
        dropdown.bind(on_select=lambda instance, x: self.COM_select(x))

        #--------------------------------
        data_input = FloatInput(size_hint=(1, 0.66))
        data_input.bind(text=self.on_text)
        #------------------------------
        send_data=Button(text='Go to [nm]:', width=350,size_hint=(1, 1))
        send_data.bind(on_press=self.data_send)
        #-----------------------------
        Get_Data_btn=Button(text='Check Positon', size_hint=(0.5, 0.66666),pos_hint={'x': 0, 'center_y': 0.6666666})
        Get_Data_Lab=ColorLabel(text="nm", markup=True, size_hint=(0.5, .66),pos_hint={'x': 0, 'center_y': 0.666666})
        Get_Data_btn.bind(on_press=self.OnGetWavelengthValue)
        #-----------------------------
        mainbutton = Button(text='Select COM port',size_hint=(1, 1))
        mainbutton.bind(on_release=dropdown.open)
        #----------------------------
        con_close = Button(text='COM port close',size_hint=(1, 1))
        con_close.bind(on_press=self.demand_close)
        #----------------------------
        slider_label=Label(text="Move by: nanometer", markup=True, pos_hint={'x': 0, 'center_y': 1.5}, size_hint=(1, 1))
        #----------------------------
        def OnSliderValueChange(instance,value):
            self.slider_value=value
            if(value==1):
                slider_label.text ="Move by: nanometer"
            elif(value==2):
                slider_label.text ="Move by: tenth of nanometer"
            elif(value==3):
                slider_label.text ="Move by: hundreth of nanometer"
            elif(value==4):
                slider_label.text ="Move by: thousandth of nanometer"
        data_slider=Slider(min=1, max=4,step=1,value=1, orientation='horizontal', pos_hint={'x': 0, 'center_y': 1}, size_hint=(1, 1))
        data_slider.bind(value=OnSliderValueChange)
        #-----------------------------
        auto_cal=Button(text='AutoCalibration', size_hint=(1, 1))
        auto_cal.bind(on_press=self.calibration)
        #-----------------------------
        step_down=Button(text='Step Down', size_hint=(0.5, .6666),pos_hint={'x': 0, 'center_y': 1.})
        step_down.bind(on_press=self.Go_Down)
        step_up=Button(text='Step Up', size_hint=(.5, .6666),pos_hint={'x': 0.5, 'center_y': 1.})
        step_up.bind(on_press=self.Go_Up)
        #------------------------------

        main_layout = GridLayout(cols=2)
        controls_layout = BoxLayout(orientation='vertical')
        controls_layout.add_widget(send_data)
        controls_layout.add_widget(auto_cal)
        controls_layout.add_widget(con_close)
        controls_layout.add_widget(mainbutton)

        inputs_layout = BoxLayout(orientation='vertical')
        inputs_layout.add_widget(data_input)
        getdata_layout = BoxLayout(orientation='horizontal')
        getdata_layout.add_widget(Get_Data_Lab)
        getdata_layout.add_widget(Get_Data_btn)
        inputs_layout.add_widget(getdata_layout)

        manual_layout = GridLayout(rows=2)
        manual_sublayout1=FloatLayout()
        manual_sublayout1.add_widget(slider_label)
        manual_sublayout1.add_widget(data_slider)

        manual_sublayout2=FloatLayout()
        manual_sublayout2.add_widget(step_down)
        manual_sublayout2.add_widget(step_up)

        manual_layout.add_widget(manual_sublayout1)
        manual_layout.add_widget(manual_sublayout2)
        inputs_layout.add_widget(manual_layout)


        main_layout.add_widget(controls_layout)
        main_layout.add_widget(inputs_layout)
        return main_layout
    def on_stop(self):
        self.demand_close()

MyApp().run()
