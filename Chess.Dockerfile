
FROM gcc:latest

WORKDIR /usr/src/Checkmate-Online

COPY . .

RUN make chess

CMD ["./bin/chess"]
