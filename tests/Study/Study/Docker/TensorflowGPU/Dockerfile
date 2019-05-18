FROM tensorflow/tensorflow:latest-gpu-py3-jupyter
# this dockerfile is used for tensorflow-gpu containers
MAINTAINER S. Guliaev <sguliaev@moroshka.cloud>
EXPOSE 8888
RUN useradd -m tensorflow
USER tensorflow
ENTRYPOINT ["jupyter-notebook"]
CMD ["--ip=0.0.0.0"]
