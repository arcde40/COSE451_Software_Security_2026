FROM ubuntu:22.04

ENV user cose451


RUN apt-get update
RUN apt-get -y install gcc gdb gcc-multilib git vim python3-pip gdbserver netcat
RUN python3 -m pip install --upgrade pip
RUN python3 -m pip install --upgrade pwntools


RUN adduser $user


ADD ./Class/. /home/$user/Class/
ADD ./Stage0_Debug_Me/. /home/$user/Stage0_Debug_Me/
ADD ./Stage1_Line_Cutting/. /home/$user/Stage1_Line_Cutting/
ADD ./Stage2_Store_Breach/. /home/$user/Stage2_Store_Breach/


WORKDIR /home/$user/


RUN chmod 770 /home/$user/
RUN chmod 770 /home/$user/*/
RUN chmod 770 /home/$user/Class/*/


RUN chmod 770 /home/$user/Class/*
RUN chmod 660 /home/$user/Class/*.py
RUN chmod 660 /home/$user/Class/*.c
RUN chmod 770 /home/$user/Class/*/*
RUN chmod 660 /home/$user/Class/*/*.py
RUN chmod 660 /home/$user/Class/*/*.c


RUN chmod +x /home/$user/Stage0_Debug_Me/*
RUN chmod +x /home/$user/Stage1_Line_Cutting/*
RUN chmod +x /home/$user/Stage2_Store_Breach/*



RUN chown -R $user:$user /home/$user/*


USER $user


RUN git clone https://github.com/longld/peda.git /tmp/peda
RUN echo "source /tmp/peda/peda.py" >> ~/.gdbinit
RUN echo "DONE! debug your program with gdb and enjoy"


WORKDIR /home/$user