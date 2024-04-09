
FROM gcc:latest

WORKDIR /usr/src/Checkmate-Online

COPY . .

RUN make server

CMD ["./bin/server"]
